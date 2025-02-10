// Headers and Namespaces.
#include "fstream"
#include "Pythia8/Pythia.h" // Include Pythia headers.
using namespace Pythia8; // Let Pythia8:: be implicit.

int main() { // Begin main program.

// Set up generation.

    Pythia pythia; // Declare Pythia object
    pythia.readString("WeakSingleBoson:ffbar2gmZ = on"); // Switch on process.
    pythia.readString("Beams:eCM = 8000."); // 8 TeV CM energy.
    pythia.readString("Next:numberShowEvent = 1000"); //List all 5 events
    pythia.init(); // Initialize; incoming pp beams is default.

// Set up Data Export
    ofstream mymain01OUTPUT;
    mymain01OUTPUT.open("../Outputs/mymain01OUTPUT.csv");




// EVENT LOOP
    for (int iEvent = 0; iEvent < 1000; ++iEvent){
        pythia.next(); // Generate an event. Fill event record.

        int iZ = 0;

        //PARTICLE LOOP (Loop through all particles of each event()
        for(int i=0; i < pythia.event.size(); i++) {
            cout << "i= " << i << ",id = " << pythia.event[i].id() << endl; 
            if (pythia.event[i].id() ==23) iZ = i;  // Find the id of last Z Boson in the record
        }

        //print some information for these remaining Z Bosons
            cout << "Transverse momentum = " << pythia.event[iZ].pT() << endl;
            cout << "Pseudorapidity = " << pythia.event[iZ].eta() << endl;
            cout << "Transverse Energy = " << pythia.event[iZ].eT() << endl;

        //Add this information to the Data Export

        // Event #| Traverse Momentum | Pseudorapidity
        mymain01OUTPUT << iEvent << "," << pythia.event[iZ].pT() << "," << pythia.event[iZ].eta() << "," << pythia.event[iZ].eT() << endl;

    }

    pythia.stat(); // Display additional statistics
    return 0;
}