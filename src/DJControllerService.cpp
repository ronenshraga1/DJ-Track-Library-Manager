#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
    if(cache.contains(track.get_title())){
        cache.get(track.get_title());
        return 1;
    } else{
      PointerWrapper<AudioTrack> cloned_track =  track.clone();
      if(!cloned_track){
        std::cout << " Track: " << track.get_title() 
        << " failed to clone and returning appropriate failure code without corrupting cache state"; 
         return 0;
      }
      AudioTrack* released_track = cloned_track.release();// here we relesing wrapper from controlling the track.
      released_track->load();
      released_track->analyze_beatgrid();
        PointerWrapper<AudioTrack> new_track(released_track);
      if(cache.put(std::move(new_track))){
        return -1;
      } else{
        return 0;
      }
    }
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    AudioTrack* track =cache.get(track_title);
    if(track){
        return track;
    } else{
        return nullptr;
    }
}
