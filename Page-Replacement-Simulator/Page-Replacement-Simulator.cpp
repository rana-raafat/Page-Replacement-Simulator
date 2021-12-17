#include <iostream>
#include <vector>
using namespace std;

//put fifo headers here

//put lru headers here

void optimal(int[], int, vector<int>&, int);
bool search_frames(int, vector<int>&);
int predict(int[], int, vector<int>&, int);


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
    int *reference_string = new int[string_length];

    for(int page_index = 0; page_index < string_length; page_index++)
    {
        cin >> reference_string[page_index];
    }

    cout << "\nchoose replacement algorithm { FIFO, LRU, optimal} :\n";
    string algo;
    cin >> algo;

    while(true) {
        if (algo == "FIFO") {
            cout << "\n______________________________________________________________________________________________________________________\n\n";
            //call fifo function here
            break;
        }
        else if (algo == "LRU") {
            cout << "\n______________________________________________________________________________________________________________________\n\n";
            //call lru function here
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

//--------------------------------------------------- LRU FUNCTIONS --------------------------------------------------//

//------------------------------------------------- OPTIMAL FUNCTIONS ------------------------------------------------//

void optimal(int reference_string[], int string_length, vector<int>& frames, int frames_number)
{
    int page_fault = 0;
    // Traverses through page reference string, checks if miss and hit and apply procedure accordingly
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
            frames.push_back(reference_string[page_index]);
        }
        // else fetches victim_index frame to replace page with
        else {             
            int victim_index = predict(reference_string, string_length, frames, page_index);
            frames[victim_index] = reference_string[page_index];
        }
    }
    cout << "No. of Page Faults = " << page_fault << endl;
    cout << "Frames in Final Stage:  [ ";
    for (int frame_index = 0; frame_index < frames.size(); frame_index++) {
        cout << frames[frame_index] << " ";
    }
    cout << "]" << endl;
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
    // initially is first index of page reference string from which to be used in the future
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
