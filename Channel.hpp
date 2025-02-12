#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <cstddef> // For size_t

// Forward declaration for Client class.
class Client;

class Channel {
public:
    // Default constructor
    Channel();

    // Parameterized constructor: creates a channel with a given name.
    Channel(const std::string& name);

    // Copy constructor (orthodox canonical form)
    Channel(const Channel& other);

    // Copy assignment operator (orthodox canonical form)
    Channel& operator=(const Channel& other);

    // Destructor (orthodox canonical form)
    ~Channel();

    // Getter for the channel name.
    const std::string& getName() const;

    // Add a client to the channel.
    // If this is the first client, they become the operator.
    void addClient(Client* client);

    // Remove a client from the channel.
    // If the client was the operator, reassign operator to another client if available.
    void removeClient(Client* client);

    // Check if the given client is the channel operator.
    bool isOperator(Client* client) const;

    // Kick a target client from the channel.
    // Only works if 'sender' is the operator.
    // Returns true if the kick was successful.
    bool kick(Client* sender, Client* target);

private:
    std::string _name;

    // Dynamic array of pointers to Client objects.
    Client** _clients;
    size_t _clientCount;    // Number of clients currently in the channel.
    size_t _clientCapacity; // Current capacity of the _clients array.

    Client* _operator;      // Pointer to the current channel operator.

    // Helper function to expand the dynamic array when needed.
    void expandCapacity();
};

#endif // CHANNEL_HPP
