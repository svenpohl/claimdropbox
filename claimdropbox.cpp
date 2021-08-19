#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp> // tapos...
//#include <eosio/crypto.h>        // sha...




#include "claimdropbox.hpp"     

 
/*
claimdropbox. (08.aug.2021 - Beginn)

08.aug.2021  - Begin

todo:

*/

using namespace eosio;
using namespace std;


 
    


const std::string   version   = "V1.0";
 

CONTRACT claimdropbox : public eosio::contract {

  public:
      using contract::contract;
      
      // ---
      // Constants
      //
      const uint32_t     hashwert    = 55342224;      
      
      
      // ---
      // struct global_s
      //
      struct [[eosio::table]]  global_s
      {
      uint64_t id; 
      uint64_t cntdrops; 
      
      auto primary_key() const { return id; }
      EOSLIB_SERIALIZE(global_s, (id)(cntdrops) )
      }; // struct global      
      
      typedef eosio::multi_index< name("global"), global_s> globals_t;
      
       
      // ---
      // struct approvals
      //
      struct [[eosio::table]] drops_s
      { 
      uint64_t      id;    
      uint64_t      asset_id;     
      name          sender;     
      name          recipient;

      
      auto primary_key()      const { return id; } 
      uint64_t by_sender()    const { return sender.value; }      
      uint64_t by_recipient() const { return recipient.value; }      
      
      EOSLIB_SERIALIZE(drops_s, (id)(asset_id)(sender)(recipient) )

      }; //struct deposits
        
      typedef eosio::multi_index <name("drops"), drops_s ,    
              indexed_by <name("sender"), const_mem_fun <drops_s, uint64_t, &drops_s::by_sender>>,          
              indexed_by <name("recipient"), const_mem_fun <drops_s, uint64_t, &drops_s::by_recipient>>   > drops_t;
  
     
          
    
      
        

      struct transfer_args
      {
      name from;
      name to;
      asset quantity;
      std::string memo;
      };    

      struct transfer_args_nft
      {
      name from;
      name to;
      vector <uint64_t> sender_asset_ids;
      std::string memo;
      };    
    
    
    
           
      
      // ---
      // Status - minimal state
      [[eosio::action]]  
      void status() 
      {
      require_auth(get_self());
      
      print("  - ");
      print(" VERSION claimdropbox: ",version," ");
      
   
      vector <uint64_t> sender_asset_id;
   
      sender_asset_id.push_back(1099558621295);
   

  
      std::string memo = "Bonus aug.12th";
          
      action(
            permission_level{_self, name("active")},
            name("atomicassets"), name("transfer"),
            std::make_tuple(_self, name("astrolympwax"), sender_asset_id, memo )
            ).send();
      
      
      print(" FIN ");
      } // status() 
    
      
      
      
      inline void splitMemo(std::vector<std::string> &results, std::string memo)
            {
            auto end = memo.cend();
            auto start = memo.cbegin();

            for (auto it = memo.cbegin(); it != end; ++it)
                {
                if (*it == ',')
                   {
                   results.emplace_back(start, it);
                   start = it + 1;
                   }
                }
            if (start != end)
            results.emplace_back(start, end);
            } // splitMemo



      
      // ---
      // handle_transfer - xxx.
      //
      [[eosio::action]]  
      void handletransf()
      {      
      auto data = unpack_action_data<transfer_args_nft>();

      print(" from: ", data.from );
      print(" to: ", data.to );

      auto size = data.sender_asset_ids.size();
      print(" size: ", size , " ");
      print(" assetid: ", data.sender_asset_ids[0], " " );
      
      print(" memo: ", data.memo , " ");
      
      /*
      uint64_t      id;    
      uint64_t      asset_id;     
      name          sender;     
      name          recipient;
      */
      
      
      std::vector<std::string> results;
      splitMemo(results, data.memo);
      auto size2 = results.size();
      
      print(" size2: ", size2 , " ");
      
      // Only send to this contract
      if ( data.to != get_self() ) 
         {
         print(" NOT SELF ");
         return;
         }
      

      check( size == size2  , " Inconsistent asset - recipient relation "); 
        
      
      for (int i=0; i<size; i++)
          {          
          //
          // Get globals
          //
          int id = 0;
          globals_t myglobals(_self, _self.value);
          auto iterator_globals = myglobals.find(id);

          uint64_t cntdrops = iterator_globals->cntdrops;      
          cntdrops++;
          print(" cntdrops: ", cntdrops , " ");
          
          
          
          
          drops_t mydrops(_self, _self.value);
          
          mydrops.emplace(_self, [&](auto&  tupel) 
                {                
                tupel.id                = cntdrops;                
                tupel.asset_id          = data.sender_asset_ids[i];                
                tupel.sender            = data.from;                
                tupel.recipient         = name( results[i] ); // ...                
                                                                           
                }); 
                
                
          
          
          //
          // Update globals
          //                                
          myglobals.modify(iterator_globals, _self, [&](auto& global) 
                   {                                                                               
                   global.cntdrops       = cntdrops;                                                                                                
                   });    
          
          } // i..
           
          
      
      print(" FIN ") ;
      } // handle_transfer
      
      
      
      
      
 
      
      //
      // canceldrop
      //
      [[eosio::action]]  
      void canceldrop( name sender, uint64_t dropid ) 
      {
      print(" CANCELDROP sender:" , sender , " ID:" , dropid , " ");
      
      require_auth( sender );
      
      
      drops_t mydrops(_self, _self.value);
      auto iterator_drops_t = mydrops.find( dropid );
      
    
      /*
      uint64_t      id;    
      uint64_t      asset_id;     
      name          sender;     
      name          recipient;
      */
      if ( iterator_drops_t != mydrops.end() )
         {
         print(" OK, FOUND! ", iterator_drops_t->asset_id);
         
         if ( iterator_drops_t->sender == sender )
            {
            // send asset
            vector <uint64_t> sender_asset_id;
   
            sender_asset_id.push_back( iterator_drops_t->asset_id );
            std::string memo = "canceldrop refund";
          
            action(
                  permission_level{_self, name("active")},
                  name("atomicassets"), name("transfer"),
                  std::make_tuple( _self, iterator_drops_t->sender, sender_asset_id, memo )
                  ).send();
            
            // delete row           
            mydrops.erase( iterator_drops_t ); 
            } //
         
         
         } // if found
         else
             {
             print(" NOT FOUND! ");
             }
      
      print(" FIN ");
      } // canceldrop

      
      
      //
      // canceldrop
      //
      [[eosio::action]]  
      void claim( name recipient, uint64_t dropid ) 
      {
      print(" CLAIM recipient:" , recipient , " ID:" , dropid , " ");
      
      require_auth( recipient );
      
      
      drops_t mydrops(_self, _self.value);
      auto iterator_drops_t = mydrops.find( dropid );
      
    
       
      if ( iterator_drops_t != mydrops.end() )
         {
         print(" OK, FOUND! ", iterator_drops_t->asset_id);
         
         if ( iterator_drops_t->recipient == recipient )
            {
            // send asset
            vector <uint64_t> sender_asset_id;
   
            sender_asset_id.push_back( iterator_drops_t->asset_id );
            std::string memo = "claim drop";
          
            action(
                  permission_level{_self, name("active")},
                  name("atomicassets"), name("transfer"),
                  std::make_tuple( _self, iterator_drops_t->recipient, sender_asset_id, memo )
                  ).send();
            
            // delete row
            
            mydrops.erase( iterator_drops_t ); 
            } //
         
         
         } // if found
         else
             {
             print(" NOT FOUND! ");
             }
      
      print(" FIN ");
      } // claim

      
      
      
       
      
      [[eosio::action]]  
      void admin() 
      {
      require_auth(get_self());

      
      print(" Admin: ",version," ");
      
 
      
      print(" FIN ");
 
      } // status() 
      
      
      

 
     
  
       
      
      
      
}; // CONTRACT claimdropbox      




extern "C"
{

void apply(uint64_t receiver, uint64_t code, uint64_t action) 
{
 
 
    if (action == name("status").value)
       {
       execute_action(name(receiver), name(code), &claimdropbox::status);  
       } 
 

    if (action == name("canceldrop").value)
       {
       execute_action(name(receiver), name(code), &claimdropbox::canceldrop);  
       } 

    if (action == name("claim").value)
       {
       execute_action(name(receiver), name(code), &claimdropbox::claim);  
       } 
 
 
     if (action == name("admin").value)
       {
       execute_action(name(receiver), name(code), &claimdropbox::admin);  
       } 
   
    
    
 
    if (
       (code == name("atomicassets").value) &&
       action == name("transfer").value
       )
       {       
       execute_action(name(receiver), name(code), &claimdropbox::handletransf);  
       }
       
       
 
 


   

} // apply
    
    
    
       
} // extern "C"      

