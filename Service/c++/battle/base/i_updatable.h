#ifndef __battle__i_updatable__
#define __battle__i_updatable__

namespace battle
{
	class i_updatable{
	public:
        i_updatable(){
            
        }
        
		virtual ~i_updatable(){

		}

		virtual void update() = 0;
	};
}

#endif // __battle__i_updatable__
