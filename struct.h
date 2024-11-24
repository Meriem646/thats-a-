#ifndef STRUCT_H
#define STRUCT_H

#define MAX_RECORDS 10   // Max number of records per block (you can adjust this as needed)
#define MAX_CUSTOMERS 100 // Max number of customers allowed in the system
#define MAX_GAMES 100     // Max number of games allowed in the system
#define MAX_RENTALS 100   // Max number of rentals allowed in the system
#include <stdio.h>
#define MAX_INDEXES 1000 // Maximum number of indexes, adjust as needed

// Structure for TOF (Tape-Oriented File)
typedef struct {
    int nbbloc;  // Number of blocks
    int lastid;  // Last used ID
} Entete;

// Structure for TOF file handling
typedef struct {
    char filename[100]; // File name for the TOF
    Entete ent;         // Header for the TOF
    FILE *f;            // File pointer
} TOF;

// Structure for Customer data
typedef struct {
    int customerID;        // Customer ID
    char firstName[50];    // Customer's first name (max 50 characters)
    char lastName[50];     // Customer's last name (max 50 characters)
    char contactInfo[100]; // Customer's contact information (max 100 characters)
    int numberOfGames;     // Number of games rented by the customer
    float totalPrice;      // Total price for the rented games
} Customer;

// Structure for Game data
typedef struct {
    int gameID;        // Game ID
    char title[100];   // Game title (max 100 characters)
    float price;       // Game price
} Game;

typedef struct {
    char firstName[50];    // Customer's first name (max 50 characters)
    char lastName[50];     // Customer's last name (max 50 characters)
    char contactInfo[100]; // Customer's contact information (max 100 characters)
    int customerID;
    int gameID;         // Associated game ID
    int rentalID;
    int numberOfGames;
    float totalPrice;
    Customer customer;  // Association entre RentalR et Customer
     char rentalDate[11]; // Rental date (format: YYYY-MM-DD, max 10 characters + null terminator)
    char returnDate[11];
    int gameIDs[MAX_GAMES];
}RentalR;

// Structure for Rental data
typedef struct {
    int rentalID;       // Rental ID
    int customerID;     // Associated customer ID
    int gameID;         // Associated game ID
    char rentalDate[11]; // Rental date (format: YYYY-MM-DD, max 10 characters + null terminator)
    char returnDate[11]; // Return date (format: YYYY-MM-DD)
    float price;
     float totalPrice;
     int numberOfGames;
      char contactInfo[100];
} Rental;

// Block structure for customers
typedef struct {
    Customer tabCust[MAX_CUSTOMERS]; // Array of customers (size defined by MAX_CUSTOMERS)
    int nbr;                         // Number of customers in the block
} BlocCustomer;

// Block structure for games
typedef struct {
    Game tabGame[MAX_GAMES]; // Array of games (size defined by MAX_GAMES)
    int nbr;                 // Number of games in the block
} BlocGame;

// Block structure for rentals
typedef struct {
    RentalR tabRental[MAX_RENTALS]; // Array of rentals (size defined by MAX_RENTALS)
    int nbr;                       // Number of rentals in the block
} BlocRentalR;



// Sparse Index on rentalID
typedef struct {
    int rentalID;       // Rental ID
    int blocIndex;      // The block index where the rental is stored
    int rentalIndex;    // The index within the block
} SparseIndex;

// Dense Index on customer name
typedef struct {
    char fullName[100]; // Full name (first name + last name)
    int customerID;     // Customer ID
} DenseIndex;

// Structures for Indexes
typedef struct {
    SparseIndex sparseIndexes[MAX_INDEXES]; // Array for sparse indexes
    int sparseCount;                        // Number of sparse indexes
} SparseIndexBlock;

typedef struct {
    DenseIndex denseIndexes[MAX_CUSTOMERS]; // Array for dense indexes
    int denseCount;                         // Number of dense indexes
} DenseIndexBlock;




#endif


