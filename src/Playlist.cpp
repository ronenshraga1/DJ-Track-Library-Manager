#include "Playlist.h"
#include "AudioTrack.h"
#include "PointerWrapper.h"
#include <iostream>
#include <algorithm>
Playlist::Playlist(const std::string& name) 
    : head(nullptr), playlist_name(name), track_counter(0) {
    std::cout << "Created playlist: " << name << std::endl;
}
// TODO: Fix memory leaks!
// Students must fix this in Phase 1
Playlist::~Playlist() {
    #ifdef DEBUG
    std::cout << "Destroying playlist: " << playlist_name << std::endl;
    #endif
    PlaylistNode* current = head;
    while(current){
        PlaylistNode* nxt = current->next;
        delete current->track;
        delete current;
        current = nxt;
    }
    track_counter=0;
    head = nullptr;
}
Playlist::Playlist(const Playlist& other)
    : head(nullptr),
      playlist_name(other.playlist_name),
      track_counter(0) {
    PlaylistNode* tail = nullptr;
    for (PlaylistNode* current = other.head; current != nullptr; current = current->next) {
        AudioTrack* original = current->track;
        AudioTrack* cloned_track = nullptr;
        if (original) {
            PointerWrapper<AudioTrack> tmp = original->clone();
            cloned_track = tmp.release();  // this playlist will own it
        }
        PlaylistNode* new_node = new PlaylistNode(cloned_track);
        if (!head) {
            head = new_node;
        } else {
            tail->next = new_node;
        }
        tail = new_node;
        ++track_counter;
    }
}
Playlist& Playlist::operator=(const Playlist& other) {
    if (this == &other) {
        return *this;
    }
    PlaylistNode* current = head;
    while (current) {
        PlaylistNode* next = current->next;
        delete current->track;
        delete current;
        current = next;
    }
    head = nullptr;
    track_counter = 0;
    playlist_name = other.playlist_name;
    PlaylistNode* tail = nullptr;
    for (PlaylistNode* source = other.head; source != nullptr; source = source->next) {
        AudioTrack* original = source->track;
        AudioTrack* cloned_track = nullptr;
        if (original) {
            PointerWrapper<AudioTrack> temp = original->clone();
            cloned_track = temp.release();
        }
        PlaylistNode* new_node = new PlaylistNode(cloned_track);
        if (!head) {
            head = new_node;
        } else {
            tail->next = new_node;
        }
        tail = new_node;
        ++track_counter;
    }
    return *this;
}



void Playlist::add_track(AudioTrack* track) {
    if (!track) {
        std::cout << "[Error] Cannot add null track to playlist" << std::endl;
        return;
    }

    // Create new node - this allocates memory!
    PlaylistNode* new_node = new PlaylistNode(track);

    // Add to front of list
    new_node->next = head;
    head = new_node;
    track_counter++;

    std::cout << "Added '" << track->get_title() << "' to playlist '" 
              << playlist_name << "'" << std::endl;
}

void Playlist::remove_track(const std::string& title) {
    PlaylistNode* current = head;
    PlaylistNode* prev = nullptr;

    // Find the track to remove
    while (current && current->track->get_title() != title) {
        prev = current;
        current = current->next;
    }

    if (current) {
        // Remove from linked list
        if (prev) {
            prev->next = current->next;
        } else {
            head = current->next;
        }
        delete current->track;
        delete current;
        track_counter--;
        std::cout << "Removed '" << title << "' from playlist" << std::endl;

    } else {
        std::cout << "Track '" << title << "' not found in playlist" << std::endl;
    }
}

void Playlist::display() const {
    std::cout << "\n=== Playlist: " << playlist_name << " ===" << std::endl;
    std::cout << "Track count: " << track_counter << std::endl;

    PlaylistNode* current = head;
    int index = 1;

    while (current) {
        std::vector<std::string> artists = current->track->get_artists();
        std::string artist_list;

        std::for_each(artists.begin(), artists.end(), [&](const std::string& artist) {
            if (!artist_list.empty()) {
                artist_list += ", ";
            }
            artist_list += artist;
        });

        AudioTrack* track = current->track;
        std::cout << index << ". " << track->get_title() 
                  << " by " << artist_list
                  << " (" << track->get_duration() << "s, " 
                  << track->get_bpm() << " BPM)" << std::endl;
        current = current->next;
        index++;
    }

    if (track_counter == 0) {
        std::cout << "(Empty playlist)" << std::endl;
    }
    std::cout << "========================\n" << std::endl;
}

AudioTrack* Playlist::find_track(const std::string& title) const {
    PlaylistNode* current = head;

    while (current) {
        if (current->track->get_title() == title) {
            return current->track;
        }
        current = current->next;
    }

    return nullptr;
}

int Playlist::get_total_duration() const {
    int total = 0;
    PlaylistNode* current = head;

    while (current) {
        total += current->track->get_duration();
        current = current->next;
    }

    return total;
}

std::vector<AudioTrack*> Playlist::getTracks() const {
    std::vector<AudioTrack*> tracks;
    PlaylistNode* current = head;
    while (current) {
        if (current->track)
            tracks.push_back(current->track);
        current = current->next;
    }
    return tracks;
}