#include "ResourceController.h"


void execute()
{   
        
    switch (mState):
    {
        case: NON_ACTIVE
        {
            // TODO read the date and see if it is time to start
            // Run starting operation 
            // Read the file and see if any of the doors should be set to opened
            // If yes set the doors 
            // Set state to active
        }
        case: ACTIVE
        {
                // Do nothing since action will be taken in the handle_callback
        }
    }
}

void handle_callback(Common::CommandContext context, void* msg)
{
    // If the state is non active return
    // Else
    // Read the time
    // Find if the door triggered is later or equal to current date and all door previous to that 
    // have been opened
    // If yes
    // Find if that door hasn't been opened yet
    // If not trigger
        // open the door
        // Update the array of the doors set and write to the file
        // If the door opened is dec 24 trigger final song
        // Set the state to NON_ACTIVe
    // If door has been opened already - do nothing
    // If the door triggered is later than current date
        // Trigger wrong door action

        

}