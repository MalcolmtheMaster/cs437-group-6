#include "EventManager.h"


bool EventManager::AlreadyRegistered( EVENTID eventId, Entity* object){
    
    //start off not registered
    bool alreadyRegistered = false;

    //iterator
    std::pair<std::multimap<EVENTID, Entity*>::iterator,
    std::multimap<EVENTID, Entity*>::iterator> range;

    //all objects that recieve this event
    range = db.equal_range(eventId);

    for (std::multimap<EVENTID, Entity*>::iterator i = range.first; 
        i != range.second; i++){
            
            //if object already subscribed 
            if((*i).second == object){
                alreadyRegistered = true;
                break;
            }
    }

	return alreadyRegistered;
}

//iterate through list of objects to respond to a specific event
void EventManager::DispactchEvent(Event &event){

  //iterator 
    std::pair<std::multimap<EVENTID, Entity*>::iterator,
    std::multimap<EVENTID, Entity*>::iterator> range;

    //find all objects who react to this event
    range = db.equal_range( event.EventId() );

    for ( std::multimap<EVENTID, Entity*>::iterator i = range.first;
        i != range.second; i++){

            //for each object handle the event
            //(*i).second ->HandleEvent(event);
        }
}

    //returns an instance of event manager
EventManager* EventManager::Instance(){
    static EventManager instance;
    return &instance;
}

//register object to listen for an event
void EventManager::RegisterObject(EVENTID event, Entity *object){

    //if object is null or it already reigstered, do nothing
    if (!object || AlreadyRegistered(event, object)){
        return;
    }

    //add the object to the database
    db.insert(std::make_pair(event, object));

}

//unregister an object from an event
void EventManager::UnregisterObject(EVENTID event, Entity *object){

    std::pair<std::multimap<EVENTID, Entity*>::iterator,
    std::multimap<EVENTID, Entity*>::iterator> range;

    range = db.equal_range( event );

    //for each object that can react to the event 
    for ( std::multimap<EVENTID, Entity*>::iterator i = range.first;
        i != range.second; i++){
        //find the object and remove 
        if ( (*i).second == object){
            //delete from database 
            i = db.erase(i);
            break;
        }
    }
}

//unregister an object from all event
void EventManager::UnregisterAll(Entity *object){
    //iterator
    std::multimap<EVENTID, Entity*>::iterator i = db.begin();

    while (i != db.end()){

        //find the correct object
        if ( (*i).second == object){

            //delete
            i = db.erase(i);
        }

        else{
            i++;
        }
    }

}

//send event
void EventManager::SendEvent(EVENTID event, void *data){
    //Event newEvent(event, data);
    //currentEvents.push_back(newEvent);
}

void EventManager::ProcessEvents(){

    //for each event in list, dispatch the event
    while (currentEvents.size()){
        DispactchEvent(*currentEvents.front());
        //then remove from the list
        currentEvents.pop_front();
    }

}

void EventManager::ClearEvents(){
    //clear the vector and lists 
    currentEvents.clear();

}

//shuts down the event manager
void EventManager::Shutdown(){
}

void EventManager::process()
{
    while (!currentEvents.empty())
    {
      dispatch(*currentEvents.front());
      //then remove from the list
      currentEvents.pop_front();
    }
}

void EventManager::dispatch(Event &event)
{
  auto type = event.getEventType();
  if (type == EventGoldCollection::eventType)
  {
    auto range = database.equal_range(type);
    for (auto i = range.first; i != range.second; ++i)
    {
      auto entity = (*i).second.first;
      auto callback = (*i).second.second;
      (entity->*callback)(event);
    }
  }
}

void EventManager::unregister(EventType e, Entity *obj)
{
  auto range = database.equal_range(e);
  for (auto i = range.first; i != range.second; ++i){
    if ((*i).second.first->get_id() == obj->get_id()){
      i = database.erase(i);
      return;
    }
  }
}
