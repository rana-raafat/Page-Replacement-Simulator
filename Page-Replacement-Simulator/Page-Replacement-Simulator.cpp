#include <iostream>
#include <vector>
using namespace std;

//FIFO headers
void fifo(int[], int, vector<int>&, int);
int victimFrameFIFO(vector<int>&, vector<int>&);

//LRU headers
void lru(int[], int, vector<int>&, int);
int victimFrameLRU(vector<int>& frames, vector<int>& counter);

//optimal headers
void optimal(int[], int, vector<int>&, int);
int predict(int[], int, vector<int>&, int);

//shared functions
bool search_frames(int, vector<int>&);
void printResult(int, vector<int>&);

int main()
{
    cout << "Enter number of frames: \n";
    int frames_number;
    cin >> frames_number;

    vector<int> frames; //initially empty

    cout << "\nEnter length of page reference string :\n";
    int string_length;
    cin >> string_length;

    cout << "\nEnter page reference string :\n";
    int* reference_string = new int[string_length];

    for (int page_index = 0; page_index < string_length; page_index++)
    {
        cin >> reference_string[page_index];
    }

    cout << "\nchoose replacement algorithm { FIFO, LRU, optimal} :\n";
    string algo;
    cin >> algo;

    while (true) {
        if (algo == "FIFO") {
            cout << "\n______________________________________________________________________________________________________________________\n\n";
            fifo(reference_string, string_length, frames, frames_number);
            break;
        }
        else if (algo == "LRU") {
            cout << "\n______________________________________________________________________________________________________________________\n\n";
            lru(reference_string, string_length, frames, frames_number);
            break;
        }
        else if (algo == "optimal") {
            cout << "\n______________________________________________________________________________________________________________________\n\n";
            optimal(reference_string, string_length, frames, frames_number);
            break;
        }
        else {
            cout << "\nCheck spelling and try again :\n";
            cin >> algo;
        }
    }
}

//--------------------------------------------------- FIFO FUNCTIONS --------------------------------------------------//

void fifo(int reference_string[], int string_length, vector<int>& frames, int frames_number) {
    int nextVictim = 0, pageFaults = 0;
    vector<int> counter;
    for (int i = 0; i < frames_number; i++) {
        counter.push_back(0);
        frames.push_back(0);
    }

    for (int i = 0; i < string_length; i++) {
        if (!search_frames(reference_string[i], frames)) {
            nextVictim = victimFrameFIFO(frames, counter);
            frames[nextVictim] = reference_string[i];
            pageFaults++;
            cout << "[ ";
            for (unsigned j = 0; j < frames.size(); j++) {
                cout << frames[j];
                if (j != frames.size() - 1) {
                    cout << " , ";
                }
            }
            cout << " ]" << endl;
        }
        for (int j = 0; j < counter.size(); j++) {
            counter[j]++;
        }
    }
    printResult(pageFaults, frames);
}

int victimFrameFIFO(vector<int>& frames, vector<int>& counter) {
    int victim = 0, max = counter[0];
    for (unsigned i = 0; i < frames.size(); i++) {
        if (frames[i] == 0) {
            for (int j = i; j >= 0; j--) {
                counter[j]++;
            }
            return i;
        }
    }
    for (unsigned i = 1; i < counter.size(); i++) {
        if (counter[i] > max) {
            max = counter[i];
            victim = i;
        }
    }
    counter[victim] = 0;

    return victim;
}

//--------------------------------------------------- LRU FUNCTIONS --------------------------------------------------//

void lru(int reference_string[], int string_length, vector<int>& frames, int frames_number) {
    int nextVictim = 0, pageFaults = 0;
    vector<int> counter;
    for (int i = 0; i < frames_number; i++) {
        counter.push_back(0);
        frames.push_back(0);
    }

    for (int i = 0; i < string_length; i++) {
        if (!search_frames(reference_string[i], frames)) {
            nextVictim = victimFrameFIFO(frames, counter);
            frames[nextVictim] = reference_string[i];
            pageFaults++;
            cout << "[ ";
            for (unsigned j = 0; j < frames.size(); j++) {
                cout << frames[j];
                if (j != frames.size() - 1) {
                    cout << " , ";
                }
            }
            cout << " ]" << endl;
        }
        else {
            for (int j = 0; j < frames.size(); j++) {
                if (frames[j] == reference_string[i]) {
                    counter[j] = 0;
                    break;
                }
            }
        }
        for (int j = 0; j < counter.size(); j++) {
            counter[j]++;
        }
    }
    printResult(pageFaults, frames);
}


int victimFrameLRU(vector<int>& frames, vector<int>& counter) {
    for (unsigned i = 0; i < frames.size(); i++) { //finding an empty frame
        if (frames[i] == 0) {
            for (int j = i; j >= 0; j--) {
                counter[j]++;
            }
            return i;
        }
    }
    int victim = 0, max = counter[0];
    for (unsigned i = 1; i < counter.size(); i++) { //find the one that's been there the longest
        if (counter[i] > max) {
            max = counter[i];
            victim = i;
        }
    }
    counter[victim] = 0;
    return victim;
}
//------------------------------------------------- OPTIMAL FUNCTIONS ------------------------------------------------//

void optimal(int reference_string[], int string_length, vector<int>& frames, int frames_number)
{
    int page_fault = 0;
    // Traverses through page reference string, checks if miss or hit and apply procedure accordingly
    for (int page_index = 0; page_index < string_length; page_index++) {
        // search() will return TRUE if page is found in one of the frames : Page Hit
        if (search_frames(reference_string[page_index], frames)) {
            //breaks current page reference iteration and continues with the next one in the loop
            continue;
        }

        // Proceeds with iteration if search() returns FALSE: page is not found in a frame : Page Miss : Page Fault

        page_fault++;

        // If there are free frames
        if (frames.size() < frames_number) {
            // insert page reference
            frames.push_back(reference_string[page_index]);
        }
        // else fetches victim frame index to replace the desired page reference into
        else {
            int victim_index = predict(reference_string, string_length, frames, page_index);
            frames[victim_index] = reference_string[page_index];
        }
        cout << "[ ";
        for (unsigned j = 0; j < frames.size(); j++) {
            cout << frames[j];
            if (j != frames.size() - 1) {
                cout << " , ";
            }
        }
        cout << " ]" << endl;
    }
    printResult(page_fault, frames);
}


bool search_frames(int page, vector<int>& frames)
{
    for (int frame_index = 0; frame_index < frames.size(); frame_index++) {
        if (frames[frame_index] == page) {
            return true;
        }
    }
    return false;
}


/* Function that checks which frame will not be referenced recently in the future- that
   frame is declared the victim according to the optimal page replacement algorithm...  */
int predict(int reference_string[], int string_length, vector<int>& frames, int current_page_refernce_index)
{
    // will later be updated with index of page reference least recently used in future
    // initially is first position of page reference string from which to be used in the future
    int farthest_used_index = current_page_refernce_index + 1;

    // may also be updated later, initially is the first frame 
    int victim_index = 0;

    int reference_index;

    // compares each frame value
    for (int frame_index = 0; frame_index < frames.size(); frame_index++) {
        // with future references
        for (reference_index = current_page_refernce_index + 1; reference_index < string_length; reference_index++) {
            // if frame value is found in the hereafter referenced pages 
            if (frames[frame_index] == reference_string[reference_index]) {
                // and page is placed farther than the current farthest position
                if (reference_index > farthest_used_index) {
                    // farthest position is updated
                    farthest_used_index = reference_index;
                    // frame is set as the newly chosen victim
                    victim_index = frame_index;
                }
                // if it's found but is NOT placed farther than the current farthest, we move on onto the next frame
                break;
            }
        }
        // in case frame is never referenced in the future, it is returned as victim without the need for further checking
        if (reference_index == string_length) {
            return frame_index;
        }
    }

    // return victim chosen by end of iterations
    return victim_index;
}

void printResult(int pageFaults, vector<int>& frames) {
    cout << "\nNo. of Page Faults = " << pageFaults << endl;
    cout << "Frames in Final Stage:  [ ";
    for (unsigned frame_index = 0; frame_index < frames.size(); frame_index++) {
        cout << frames[frame_index];
        if (frame_index != frames.size() - 1) {
            cout << " , ";
        }
    }
    cout << " ]" << endl;
}