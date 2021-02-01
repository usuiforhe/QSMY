#ifndef __battle__game_context__
#define __battle__game_context__

#include "battle_prefix.h"
#include "singleton.h"
#include "meta/scene_meta_data.pb.h"
#include "net/protobuf_codec.h"
namespace battle{
    class report;
    class battle_request{
    public:
        battle_request(latte::tcp_conn_ptr conn, uint32_t sync_id, boost::shared_ptr<scene_meta_data> data):
            conn_(conn),
            sync_id_(sync_id),
            data_(data){
        }
        
        ~battle_request(){
            
        }
        
        scene_meta_data & get_data(){
            return *data_;
        }
        
        void send_report(report * report);
        
    private:
        latte::tcp_conn_ptr conn_;
        uint32_t sync_id_;
        boost::shared_ptr<scene_meta_data> data_;
    };
    
    class processor{
    public:
        processor();
        
        ~processor();
        
        void start();
        
        void stop();
        
        void join();
        
        void add_request(battle_request * request);
        
    private:
        bool is_stop_;
        boost::thread * thread_;
        std::list<battle_request*> requests_;
        boost::mutex locker_;
        
        void process();
        
        inline void process_one(battle_request * request);
    };
    
	class game_context{
	public:
        game_context();
        
        ~game_context();
        
        void add_request(battle_request * request);
        
        void start();
        
        void stop();
        
        void join();
        
	private:
        uint8_t processor_index_;
        uint8_t processor_count_;
        std::vector<processor*> processors_;
        
        boost::mutex locker_;
	};
}

#define S_GAME_CONTEXT latte::singleton<game_context>::instance()

#endif // __battle__game_context__
