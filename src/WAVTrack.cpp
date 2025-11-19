#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    std::cout << "[WAVTrack::load] Loading WAV: "<<title<<"at" <<sample_rate<< "Hz/"<<bit_depth<< "bit (uncompressed)...";
    long size = duration_seconds * sample_rate * (bit_depth / 8) * 2;
    std::cout <<"  → Estimated file size:"<< size << "bytes";
    std::cout <<"  → Fast loading due to uncompressed format.";

}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement WAV-specific beat detection analysis
    // Requirements:
    // 1. Print analysis message with track title
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    // 3. Print number of beats and mention uncompressed precision
    // should print "  → Estimated beats: <beats>  → Precision factor: 1.0 (uncompressed audio)"
    double beats = (duration_seconds / 60.0) *bpm;
    int precision_factor = 1;
    std::cout<<"  → Estimated beats:" <<beats<< "  → Precisionfactor: 1 (uncompressed audio)";
}

double WAVTrack::get_quality_score() const {
    // TODO: Implement WAV quality scoring
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    // NOTE: Cast beats to integer when printing
    double score = 70;
    if(sample_rate >= 44100)
        score+=10;
    if(sample_rate >=96000)
        score+=5;
    if(bit_depth>=16)
        score+=10;
    if(bit_depth>=24)
        score+=5;
    if(score >100){
        return 100;
    } else{
        return score;
    }
    
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {//must return a pointer
    // TODO: Implement the clone method
    WAVTrack* copy = new WAVTrack(*this);
    return PointerWrapper<AudioTrack>(copy); // Replace with your implementation
}