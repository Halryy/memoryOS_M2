#include <iostream>
#include <fstream>
#include <random>
#include <string>

using namespace std;

string trim(const string& str) {
    size_t inicio = str.find_first_not_of(" \t\n\r");
    size_t fim = str.find_last_not_of(" \t\n\r");

    if (inicio == std::string::npos) // A string está vazia ou contém apenas espaços em branco
        return "";

    return str.substr(inicio, fim - inicio + 1);
}

string getsPhysicalPageNumberFromTable(int virtualPageNumber, string fileName) {
    int lineNumberToAccess = virtualPageNumber;
    int currentRowNumber = 1;

    string line;
    
    ifstream inputFile(fileName);

    if (!inputFile) {
        cout << "\nError: Couldn't open the file\n" << endl;
    }


    while (currentRowNumber < lineNumberToAccess + 1 && getline(inputFile, line)) {
        currentRowNumber++;
    }

    if (currentRowNumber == lineNumberToAccess + 1) {
        return line;
    }
    return "\nError: value not found\n";
}

string getsValueFromMemory(int physicalPageNumber, int pageOffset ) {
    int lineNumberToAccess = physicalPageNumber + pageOffset;
    int currentRowNumber = 1;

    string line;
    
    ifstream inputFile("data_memory.txt");

    if (!inputFile) {
        cout << "\nError: Couldn't open the file\n" << endl;
    }


    while (currentRowNumber < lineNumberToAccess + 1 && getline(inputFile, line)) {
        currentRowNumber++;
    }

    if (currentRowNumber == lineNumberToAccess + 1) {
        return line;
    }
    return "\nError: value not found\n";
}

int createsVirtualPageNumber(int bitsQuantity) {
        if (bitsQuantity == 16) {
        mt19937 rng(random_device{}());

        // Define um distribuidor para gerar números inteiros entre 1 e 1001 (inclusive).
        uniform_int_distribution<int> distribution(1, 1000);

        // Gera um número aleatório.
        int numeroAleatorio = distribution(rng);

        return numeroAleatorio;
    } else {
        mt19937 rng(random_device{}());

        // Define um distribuidor para gerar números inteiros entre 1 e 1001 (inclusive).
        uniform_int_distribution<int> distribution(1, 1000000);

        // Gera um número aleatório.
        int numeroAleatorio = distribution(rng);

        return numeroAleatorio;
    }
}

int main(int argc, char *argv[]) {
    string inputByUser = ""; // NUMBER SET BY USER!!!
    string fileName = ""; // fileName CAN CHANGE BASED ON THE VIRTUAL MEMORY'S SIZE!!!
    string physicalPageNumber = "";
    int bitsQuantity = 16;
    int pageOffset = 256;
    bool notANumber = false;
    bool differentThan32 = false, differentThan16 = false;
    int virtualPageNumber = 0; // Can also be called by Virtual Address!

    if (argc == 2) {
        if (argv) {
            try {
                int number = stoi(argv[1]);
            } catch (const invalid_argument& e) {
                notANumber = true;
            }
            if (notANumber == false) {
                if (stoi(argv[1]) > 0 && stoi(argv[1]) < 1001) {
                    bitsQuantity = 16;
                } else {
                    bitsQuantity = 32;
                }
            }
        }
    }

    switch (argc) {

        case 2:
            if (trim(argv[1]) == "addresses_16b.txt" || trim(argv[1]) == "addresses_32b.txt") {
                // fileName CAN BE SET BY USER TO GENERATE A RANDOM VIRTUAL PAGE NUMBER (VIRTUAL ADDRESS)!!!
                fileName = trim(argv[1]);
                if (fileName == "addresses_16b.txt") {
                    bitsQuantity = 16;
                    virtualPageNumber = createsVirtualPageNumber(bitsQuantity);
                } else {
                    bitsQuantity = 32;
                    virtualPageNumber = createsVirtualPageNumber(bitsQuantity);
                }

                cout << "\nPage offset size (256 to 4096 bits): ";
                cin >> pageOffset;
                if (!cin >> pageOffset) {
                    cout << "\nError: Page offset size must be a number";
                    return 0;
                }

                if (pageOffset > 255 && pageOffset < 4097) {
                    physicalPageNumber = getsPhysicalPageNumberFromTable(virtualPageNumber, fileName);
                    // RESULT!!!
                    cout << "\nVirtual address: " << virtualPageNumber << endl;
                    cout << "Page number: " << getsPhysicalPageNumberFromTable(virtualPageNumber, fileName) << endl;
                    cout << "Page offset: " << pageOffset << endl;
                    cout << "Value read: " << getsValueFromMemory(stoi(physicalPageNumber), pageOffset) << endl;
                    return 0;
                }
                cout << "\nError: Page offset size must be a valid number";
                return 0;
            }

            // NUMBER SET BY USER!!!
            inputByUser = argv[1];

            // CHECKS IF THE USER'S VALUE IS AN INTEGER!!!
            try {
                int number = stoi(inputByUser);
            } catch (const invalid_argument& e) {
                cerr << "\nError: Unknown parameter\n";
                return 0;
            }
            // END OF THE VERIFICATION!!!

            virtualPageNumber = stoi(inputByUser); // Can also be called by Virtual Address!
            fileName = "addresses_" + to_string(bitsQuantity) + "b.txt"; // fileName CAN CHANGE BASED ON THE VIRTUAL MEMORY'S SIZE!!!

            // CHECKS IF USER'S VALUE IS A VALID VIRTUAL PAGE NUMBER (VIRTUAL ADDRESS) !!!
            if (bitsQuantity == 16) {
                if (virtualPageNumber < 1001 && virtualPageNumber > 0) {

                    physicalPageNumber = getsPhysicalPageNumberFromTable(virtualPageNumber, fileName);

                    cout << "\nVirtual memory size: " << bitsQuantity << " bits" << endl;
                    cout << "Virtual address: " << virtualPageNumber << endl;
                    cout << "Page number: " << physicalPageNumber << endl;
                    cout << "Page offset: " << pageOffset << endl;
                    cout << "Read value: " << getsValueFromMemory(stoi(physicalPageNumber), pageOffset) << endl;

                    return 0;

                } else {
                    cout << "\nError: Virtual adress does not exist!\n";
                    return 0;
                }
            } else if (bitsQuantity == 32) {
                if (virtualPageNumber > 0 && virtualPageNumber < 1000001) {
                    physicalPageNumber = getsPhysicalPageNumberFromTable(virtualPageNumber, fileName);

                    cout << "\nVirtual memory size: " << bitsQuantity << " bits" << endl;
                    cout << "Virtual address: " << virtualPageNumber << endl;
                    cout << "Page number: " << physicalPageNumber << endl;
                    cout << "Page offset: " << pageOffset << endl;
                    cout << "Read value: " << getsValueFromMemory(stoi(physicalPageNumber), pageOffset) << endl;

                    return 0;
                } else {
                    cout << "\nError: Virtual adress does not exist!\n";
                    return 0;
                }
            } else {
                cout << "Error: Virtual memory size";
                return 0;
            }
            
        break;

        case 1:
            // NO PARAMETERS SET BY USER!!!
            cout << "\nVirtual memory size (16 or 32 bits): ";
            cin >> bitsQuantity;
            if (bitsQuantity != 16) {
                differentThan16 = true;
            }
            if (bitsQuantity != 32) {
                differentThan32 = true;
            }


            if (differentThan16 == false || differentThan32 == false) {
                cout << "Page offset size (256 to 4096 bits): ";
                cin >> pageOffset;
                if (!cin >> pageOffset) {
                    cout << "\nError: Page offset size must be a number";
                    return 0;
                }
                cout << "Virtual address: ";
                cin >> virtualPageNumber;
                if (!cin >> virtualPageNumber) {
                    cout << "\nError: Virtual address must be a number";
                    return 0;
                }
                if (pageOffset > 255 && pageOffset < 4097) {
                    if (bitsQuantity == 16) {
                        if (virtualPageNumber < 1001 && virtualPageNumber > 0) {
                            // virtualPageNumber = createsVirtualPageNumber(bitsQuantity); // CAN ALSO BE CALLED BY VIRTUAL ADDRESS!
                            fileName = "addresses_" + to_string(bitsQuantity) + "b.txt"; // fileName CAN CHANGE BASED ON THE VIRTUAL MEMORY'S SIZE!!!
                            physicalPageNumber = getsPhysicalPageNumberFromTable(virtualPageNumber, fileName);

                            // RESULT !!!
                            // cout << "\nVirtual address: " << virtualPageNumber << endl;
                            cout << "Page number: " << physicalPageNumber << endl;
                            cout << "Page offset: " << pageOffset << endl;
                            cout << "Read value: " << getsValueFromMemory(stoi(physicalPageNumber), pageOffset) << endl;
                            return 0;
                        }
                        cout << "\nError: Virtual address does not exist!\n";
                        return 0;
                    } else {
                        if (virtualPageNumber < 1000001 && virtualPageNumber > 0) {
                            // virtualPageNumber = createsVirtualPageNumber(bitsQuantity); // CAN ALSO BE CALLED BY VIRTUAL ADDRESS!
                            fileName = "addresses_" + to_string(bitsQuantity) + "b.txt"; // fileName CAN CHANGE BASED ON THE VIRTUAL MEMORY'S SIZE!!!
                            physicalPageNumber = getsPhysicalPageNumberFromTable(virtualPageNumber, fileName);

                            // RESULT !!!
                            // cout << "\nVirtual address: " << virtualPageNumber << endl;
                            cout << "Page number: " << physicalPageNumber << endl;
                            cout << "Page offset: " << pageOffset << endl;
                            cout << "Read value: " << getsValueFromMemory(stoi(physicalPageNumber), pageOffset) << endl;
                        }
                        cout << "\nError: Virtual address does not exist!\n";
                        return 0;
                    }
                }
                cout << "\nError: Page offset size must be a valid number";
                return 0;
            }
            cout << "\nError: Virtual memory size\n";
            return 0;
        break;

        default:
            // ERROR!!!
            cout << "Comando inexistente/inválido!";
            return 0;
    }
}
