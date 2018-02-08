#include "Header.h";

// Initialize the MBT and srand for random number generation
// We use a reference to make sure to properly update the variable out of the function scope
void initialize(mbt &table)
{
	srand(time(NULL));
	table.size = MEMORY_SIZE;
	for (int i = 0; i < table.size; i++) {
		table.content[i] = true;
	}
}

// Display the memory block table
void showMBT(mbt const& table) {
	std::cout << "Memory Block Table size : " << table.size << std::endl;
	std::cout << "Memory Block Table content : " << std::endl;
	for (int i = 0; i < MEMORY_SIZE; i++) {
		if (table.content[i] == true) {
			std::cout << "11111111 ";
		}
		else {
			std::cout << "00000000 ";
		}
		if ((i + 1) % 8 == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	std::list<pcb> ready_queue; //ready_queue containing the PCBs, I use a list instead of a queue to iterate more easily over its content in order to display processes ID
	int pid = 0; //Process ID counter
	int randsize = 0; // Random variable
	std::string response; // String where the user input will be stored
	int value = 0; // Used to store the command number for the user imput
	mbt table; // Memory Block Table variable
	int blocks = 0; // Number of bytes corresponding to the random value
	page tab = { NULL }; // Initialise a Page table variable
	bool found = false;

	initialize(table);
	std::cout << "Space left : " << table.size * 8 << " bits" << std::endl;
	// Infinite loop
	while (true) {
		std::cout << "Please pick an option :" << std::endl << "1 : Initiate a process" << std::endl
			<< "2 : Print processes in ready queue" << std::endl << "3 : Terminate a process" << std::endl << "4 : Exit" << std::endl;
		std::cout << "Command : ";
		// We get the user input
		std::getline(std::cin, response);
		if (response != "") {
			// We convert the user input
			value = atoi(response.c_str());
			switch (value)
			{
			case 1:
				// increment pid
				pid++;
				// Get random size
				randsize = (rand() % 240) + 10;
				if (randsize > (table.size * 8)) {
					std::cout << "There is not enough space in the Memory Block Table." << std::endl;
				}
				else {
					// I calculate the number of bytes necessary for the allocation
					blocks = (randsize / 8) + ((randsize % 8) > 0 ? 1 : 0);
					// I initialize the Page table
					tab.size = blocks;
					tab.content = new bool[blocks];
					// Add the PCB to the ready Queue
					ready_queue.push_back({ pid, tab });
					// We update the MBT content
					for (int i = 0; i < blocks; i++) {
						table.content[MEMORY_SIZE - table.size + i] = false;
					}
					table.size -= blocks;
					std::cout << "Page table size : " << randsize << " bits or " << blocks << "bytes " << std::endl;
					std::cout << "PCB process id : " << pid << std::endl;
					showMBT(table);
				}
				break;
			case 2:
				// Simple iteration over the Queue to display the Processes ID
				for (auto it = ready_queue.begin(); it != ready_queue.end(); ++it) {
					std::cout << it->pid << std::endl;
				}
				break;
			case 3:
				std::cout << "Give process PID to kill : ";
				std::getline(std::cin, response);
				if (response != "") {
					value = atoi(response.c_str());
					for (auto it = ready_queue.begin(); it != ready_queue.end(); ++it) {
						if (it->pid == value) {
							// Update the MBT content in order to free the space used by the process
							for (int c = 0; c <= it->table.size; c++) {
								table.content[MEMORY_SIZE - table.size - c] = true;
							}
							table.size += it->table.size;
							delete (it->table.content); // Delete the table.content which is dynamicaly allocated on the heap
							ready_queue.erase(it);
							found = true;
							showMBT(table);
							break; // Exit the for loop
						}
					}
					if (found == false) {
						std::cout << "Process with PID " << value << " not found" << std::endl;
					}
					found = false;
				}
				else {
					std::cout << "Invalid PID" << std::endl;
				}
				break;
			case 4:
				if (ready_queue.size() != 0) {
					std::cout << "Ready queue is not empty, confirm request exit (Y/N)" << std::endl;
					std::getline(std::cin, response);
					if (response == "Y") {
						showMBT(table);
						for (auto it = ready_queue.begin(); it != ready_queue.end(); ++it) {
							std::cout << it->pid << std::endl;
							delete (it->table.content); // We delete the Page Table content which is the only one stored dynamicaly on the heap
						}
						return (0); // Exit code 0, process ended normally
					}
					else {
						break;
					}
				}
				else {
					return (0);
				}
				break;
			default:
				std::cout << "Invalid command : " << response << std::endl;
				break;
			}
		}
	}
	return (1);
}