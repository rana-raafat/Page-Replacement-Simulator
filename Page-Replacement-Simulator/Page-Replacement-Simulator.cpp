#include <iostream>
#include <vector>
using namespace std;

//FIFO headers
void fifo(int[], int, vector<int>&, int, vector<int>&);
int victimFrameFIFO(vector<int>&, vector<int>&);

//LRU headers
void lru(int[], int, vector<int>&, int, vector<int>&);
int victimFrameLRU(vector<int>&, vector<int>&);

//optimal headers
void optimal(int[], int, vector<int>&, int, vector<int>&);
int optimal_victim(int[], int, vector<int>&, int, vector<int>&);

//shared functions
void printString(int[], int);
bool search_frames(int, vector<int>&);
void printResult(int, bool, vector<int>&);

int main()
{
    cout << "Enter number of frames: \n";
    int frames_number;
    cin >> frames_number;

    vector<int> frames(frames_number, -1); //initially empty
    vector<int> counter(frames_number, 0); //initially empty

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
            printString(reference_string, string_length);
            fifo(reference_string, string_length, frames, frames_number, counter);
            break;
        }
        else if (algo == "LRU") {
            cout << "\n______________________________________________________________________________________________________________________\n\n";
            printString(reference_string, string_length);
            lru(reference_string, string_length, frames, frames_number, counter);
            break;
        }
        else if (algo == "optimal") {
            cout << "\n______________________________________________________________________________________________________________________\n\n";
            printString(reference_string, string_length);
            optimal(reference_string, string_length, frames, frames_number, counter);
            break;
        }
        else {
            cout << "\nCheck spelling and try again :\n";
            cin >> algo;
        }
    }
}

//--------------------------------------------------- FIFO FUNCTIONS --------------------------------------------------//

void fifo(int reference_string[], int string_length, vector<int>& frames, int frames_number, vector<int>& counter) {
    int nextVictim = 0, pageFaults = 0;

    for (int i = 0; i < string_length; i++) {
        if (!search_frames(reference_string[i], frames)) {
            nextVictim = victimFrameFIFO(frames, counter);
            frames[nextVictim] = reference_string[i];
            pageFaults++;
            printResult(pageFaults, false, frames);
        }
        //update occupied frames' counter
        for (unsigned j = 0; j < counter.size(); j++) {
            if(frames[j] != -1)
                counter[j]++;
        }
    }
    printResult(pageFaults, true, frames);
}


int victimFrameFIFO(vector<int>& frames, vector<int>& counter) {
    //to check if there are empty frames
    for (unsigned i = 0; i < frames.size(); i++) {
        if (frames[i] == -1) {
            return i;
        }
    }

    //if none returned, finds the one that's been there the longest
    int victim = 0, max = counter[0];
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

void lru(int reference_string[], int string_length, vector<int>& frames, int frames_number, vector<int>& counter) {
    int nextVictim = 0, pageFaults = 0;

    for (int i = 0; i < string_length; i++) {
        if (!search_frames(reference_string[i], frames)) {
            nextVictim = victimFrameLRU(frames, counter);
            frames[nextVictim] = reference_string[i];
            pageFaults++;
            printResult(pageFaults, false, frames);

        }
        else {
            for (unsigned j = 0; j < frames.size(); j++) {
                if (frames[j] == reference_string[i]) {
                    counter[j] = 0;
                    break;
                }
            }
        }

        for (unsigned j = 0; j < counter.size(); j++) {
            if (frames[j] != -1)
                counter[j]++;
        }
    }
    printResult(pageFaults, true, frames);
}


int victimFrameLRU(vector<int>& frames, vector<int>& counter) {
    for (unsigned i = 0; i < frames.size(); i++) { //finding an empty frame
        if (frames[i] == -1) {
            return i;
        }
    }
    int victim = 0, max = counter[0];
    for (unsigned i = 1; i < counter.size(); i++) { //find the one that hasn't been used for the longest
        if (counter[i] > max) {
            max = counter[i];
            victim = i;
        }
    }

    counter[victim] = 0;

    return victim;
}
//------------------------------------------------- OPTIMAL FUNCTIONS ------------------------------------------------//

void optimal(int reference_string[], int string_length, vector<int>& frames, int frames_number, vector<int>& counter)
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

        // fetches free frame or victim frame index to replace the desired page reference into
        int victim_index = optimal_victim(reference_string, string_length, frames, page_index, counter);
        frames[victim_index] = reference_string[page_index];

        printResult(page_fault, false, frames);

        for (unsigned j = 0; j < counter.size(); j++) {
            if (frames[j] != -1)
                counter[j]++;
        }
    }

    printResult(page_fault, true, frames);

}



/* Function that checks which frame will not be referenced recently in the future- that
   frame is declared the victim according to the optimal page replacement algorithm...  */
int optimal_victim(int reference_string[], int string_length, vector<int>& frames, int current_page_refernce_index, vector<int>& counter)
{

    // If there are empty frames
    for (unsigned frame_index = 0; frame_index < frames.size(); frame_index++) {
        if (frames[frame_index] == -1) {
            //  function returns its index
            return frame_index;
        }
    }

    // if nothing is returned, proceeds to choose victim

    // will later be updated with index of page reference least recently used in future
    // initially is first position in page reference string from which to be used in the future
    int farthest_used_index = current_page_refernce_index + 1;

    // will be updated later
    int victim_index = 0;

    // nested for loop iteration index
    int reference_index;

    vector<int> possible_victims;
    vector<int> possible_victims_counter;

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
        // in case frame is never referenced in the future
        if (reference_index == string_length) {

            possible_victims.push_back(frames[frame_index]);
            possible_victims_counter.push_back(counter[frame_index]);

            // if there's another frame previously detected that is also never referenced in the future
            if (farthest_used_index == string_length + 1) {
                int temp_index = victimFrameFIFO(possible_victims, possible_victims_counter);
                for (unsigned i = 0; i < frames.size(); i++) {
                    if (frames[i] == possible_victims[temp_index]) {
                        victim_index = i;
                        break;
                    }
                }
            }
            //else no need to check
            else{
                victim_index = frame_index;
                farthest_used_index = string_length + 1;
            }
        }
    }

    counter[victim_index] = 0;

    // return victim by end of iterations
    return victim_index;
}

//--------------------------------------------------- SHARED FUNCTIONS --------------------------------------------------//

void printString(int reference_string[], int string_length) {
    cout << "The reference string is { ";
    for (int page_index = 0; page_index < string_length; page_index++) {
        cout << reference_string[page_index];
        if (page_index != string_length - 1) {
            cout << " , ";
        }
    }
    cout << " }\n\n";
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


void printResult(int pageFaults, bool final_stage, vector<int>& frames) {
    if (final_stage) {
        cout << "\nNumber of Page Faults = " << pageFaults << endl;
        cout << "Frames in Final Stage: ";
    }
    cout << "[ ";
    for (unsigned frame_index = 0; frame_index < frames.size(); frame_index++) {
        cout << frames[frame_index];
        if (frame_index != frames.size() - 1) {
            cout << " , ";
        }
    }
    cout << " ]" << endl;
}
