#include "game_context.h"
#include "config.h"
#include "builder/scene_builder.h"
#include "scene.h"
#include "report.h"
#include "net/tcp_connection.h"
#include "gateway/battle_service.h"

using namespace battle;

void battle_request::send_report(battle::report * report){
    boost::shared_ptr<google::protobuf::Message> message = report->to_message();
    latte::header_protobuf_message_ptr pm(new latte::header_protobuf_message(message, sync_id_, sync_id_ > 0 ? true : false));
    
    S_BATTLE_SERVICE.send(conn_, pm);
}

processor::processor():thread_(NULL){

}

processor::~processor(){
    is_stop_ = true;
    if(thread_ != NULL){
        delete thread_;
    }
}

void processor::start(){
    thread_ = new boost::thread(boost::bind(&processor::process, this));
}

void processor::stop(){
    is_stop_ = true;
}

void processor::join(){
    thread_->join();
}

void processor::add_request(battle_request *request){
    requests_.push_back(request);
}

void processor::process(){
    battle_request * request = NULL;
    while(!is_stop_){
        {
            boost::mutex::scoped_lock(locker_);
            if(!requests_.empty()){
                request = requests_.front();
                requests_.pop_front();
            }
            else{
                boost::this_thread::sleep(boost::posix_time::milliseconds(3));
            }
        }
        
        if(request != NULL){
            try{
                process_one(request);
            }
            catch (boost::exception &e)
            {
                std::stringstream &ss = latte::fast_stringstream::ss();
                ss << "processor::process() throws exceptions. what:" + boost::diagnostic_information(e);
                s_log.out_error(ss.str());
            }
            catch (std::exception &e)
            {
                std::stringstream &ss = latte::fast_stringstream::ss();
                ss << "processor::process() throws exceptions. what:" + boost::diagnostic_information(e);
                s_log.out_error(ss.str());
            }
            delete request;
            request = NULL;
        }
    }
}

void processor::process_one(battle_request * request){
#if DEBUG
    clock_t t_start, t_end;
    t_start = clock();
#endif
    battle::scene * scene = S_SCENE_BUILDER.build(request->get_data());
    if(scene != NULL){
        scene->execute();
        request->send_report(scene->get_report());
        delete scene;
    }
    
#if DEBUG
    t_end = clock();
    
    std::stringstream & ss = latte::fast_stringstream::ss();
    ss << "battle elapsed:" << (double)(t_end - t_start)*1000/CLOCKS_PER_SEC << "ms" << std::endl;
    s_log.out_debug(ss.str());
#endif
}

game_context::game_context()
    :processor_index_(0),
    processor_count_(S_CONFIG.get_battle_thread_count())
{
    processors_.reserve(processor_count_);
}

game_context::~game_context(){
    stop();
}

void game_context::add_request(battle_request *request){
    boost::mutex::scoped_lock(locker_);
    processors_[processor_index_]->add_request(request);
    
    if(++processor_index_ >= processor_count_){
        processor_index_ = 0;
    }
}

void game_context::start(){
    for(uint8_t i = 0; i < processor_count_; ++i){
        processors_.push_back(new processor);
    }
    BOOST_FOREACH(processor * val, processors_){
        val->start();
    }
}

void game_context::stop(){
    BOOST_FOREACH(processor * val, processors_){
        if(val != NULL)
        {
            val->stop();
        }
    }
}

void game_context::join(){
    BOOST_FOREACH(processor * val, processors_){
        val->join();
    }
}
