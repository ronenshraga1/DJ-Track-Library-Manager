#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    // Your implementation here
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout <<"[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
        std::cout <<"[MixingEngineService] Cleaning up decks..."<< std::endl;
        delete decks[0];
        delete decks[1];// decks is fixed array so its on the stack.
        decks[0] = nullptr;
        decks[1] = nullptr;
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
        std::cout<<"\n=== Loading Track to Deck ===\n";
        PointerWrapper<AudioTrack> cloned_track =  track.clone();
        if(!cloned_track){
            std::cout<< "[ERROR] Track:"<<track.get_title()<<" failed to clone";
            return -1;
        }
            size_t target_deck;
            if (!decks[0] && !decks[1]) {
                target_deck = 0;           
            } else {
                target_deck = 1 - active_deck;
            }

            std::cout<<"[Deck Switch] Target deck: "<<target_deck<<"\n";
            if(decks[target_deck]){
                delete decks[target_deck];
                decks[target_deck] =nullptr;
            }
            cloned_track.get()->load();
            cloned_track.get()->analyze_beatgrid();
            if (auto_sync) {
                if (!decks[active_deck]) {
                    std::cout << "[Sync BPM] Cannot sync - one of the decks is empty.\n";
                } else if (!can_mix_tracks(cloned_track)) {
                    sync_bpm(cloned_track);
                }
            }
            AudioTrack* releasd_track = cloned_track.release();
            decks[target_deck] = releasd_track;
            releasd_track = nullptr;
            std::cout<<"[Load Complete] '"<<decks[target_deck]->get_title()<<"' is now loaded on deck "<<target_deck<<"\n";
            active_deck = target_deck;
            std::cout<<"[Active Deck] Switched to deck "<<target_deck<<"";
            return target_deck;
        }
    

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(!decks[active_deck] || !track){
        return false;
    }
    bool is_tolerant = abs(track->get_bpm() - decks[active_deck]->get_bpm()) <= bpm_tolerance;
    return is_tolerant;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] && track){
        int original = track->get_bpm();
        int average = abs(original + decks[active_deck]->get_bpm()) /2;
        track->set_bpm(average);
        std::cout<<"[Sync BPM] Syncing BPM from "<<original<<" to "<<average<<"\n";
    }
}
