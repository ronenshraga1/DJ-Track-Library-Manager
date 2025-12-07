#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "PointerWrapper.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    // Clean any existing library content
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();

    for (const SessionConfig::TrackInfo& track_info : library_tracks) {
        AudioTrack* track = nullptr;

        if (track_info.type == "MP3") {
            bool has_tags = (track_info.extra_param2 != 0);
            track = new MP3Track(
                track_info.title,
                track_info.artists,
                track_info.duration_seconds,
                track_info.bpm,
                track_info.extra_param1,
                has_tags
            );
            std::cout << "MP3Track created: " << track_info.extra_param1 << " kbps\n";
        } else if (track_info.type == "WAV") {
            track = new WAVTrack(
                track_info.title,
                track_info.artists,
                track_info.duration_seconds,
                track_info.bpm,
                track_info.extra_param1,   
                track_info.extra_param2    
            );
            std::cout << "WAVTrack created: " << track_info.extra_param1 << "Hz/"<< track_info.extra_param2 << "bit\n";
        } else {
            std::cout << "[WARNING] Unknown track type '" << track_info.type << "' (skipping)\n";
        }

        if (track != nullptr) {
            library.push_back(track);
        }
    }

    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}
DJLibraryService::~DJLibraryService() {
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();
}
DJLibraryService::DJLibraryService(const DJLibraryService& other)
    : playlist(other.playlist),
      library()
{
    library.reserve(other.library.size());
    for (AudioTrack* source : other.library) {
        if (source) {
            PointerWrapper<AudioTrack> cloned = source->clone();
            library.push_back(cloned.release());
        } else {
            library.push_back(nullptr);
        }
    }
}
DJLibraryService& DJLibraryService::operator=(const DJLibraryService& other) {
    if (this == &other) {
        return *this;
    }
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();
    playlist = other.playlist;
    library.reserve(other.library.size());
    for (AudioTrack* source : other.library) {
        if (source) {
            PointerWrapper<AudioTrack> cloned = source->clone();
            library.push_back(cloned.release());
        } else {
            library.push_back(nullptr);
        }
    }
    return *this;
}



/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title); 
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    std::cout<<"[INFO] Loading playlist: " <<playlist_name<<std::endl;
    playlist =  Playlist(playlist_name);
    int counter = 0;
    for(size_t i =0;i<track_indices.size();i++){
        int  index = track_indices[i];
        if (index  < 1 || static_cast<size_t>(index) > library.size()){
            std::cout<<"[WARNING] Invalid track index: "<<index<<std::endl;
        } else{
            AudioTrack* base = library[index - 1];
            PointerWrapper<AudioTrack> wrapper = base->clone();
            if(wrapper){
                counter++;
                wrapper->load();
                wrapper->analyze_beatgrid();
                AudioTrack* track = wrapper.release();
                playlist.add_track(track);
                std::cout << "Added '" << track->get_title()<< "' to playlist '" << playlist_name << "'\n";

            }else{
                std::cout<<"Clone failed"<<std::endl;
            }
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name<< " (" << counter << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;
    std::vector<AudioTrack*> tracks=playlist.getTracks();
    for(size_t i =0; i<tracks.size();i++){
        titles.push_back(tracks[i]->get_title());
    }
    return titles;
}
