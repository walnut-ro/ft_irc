#include "Channel.hpp"
#include "Client.hpp"

// Define an initial capacity for the dynamic array.
static const size_t INITIAL_CAPACITY = 10;

// Default constructor: creates an open channel with an empty name.
Channel::Channel()
    : _name(""), _clientCount(0), _clientCapacity(INITIAL_CAPACITY), _operator(nullptr)
{
    _clients = new Client*[ _clientCapacity ];
}

// Parameterized constructor: creates a channel with the given name.
Channel::Channel(const std::string& name)
    : _name(name), _clientCount(0), _clientCapacity(INITIAL_CAPACITY), _operator(nullptr)
{
    _clients = new Client*[ _clientCapacity ];
}

// Copy constructor: performs a shallow copy of the client pointers.
Channel::Channel(const Channel& other)
    : _name(other._name),
      _clientCount(other._clientCount),
      _clientCapacity(other._clientCapacity),
      _operator(other._operator)
{
    _clients = new Client*[ _clientCapacity ];
    for (size_t i = 0; i < _clientCount; ++i) {
        _clients[i] = other._clients[i];
    }
}

// Assignment operator: cleans up old data and copies from other.
Channel& Channel::operator=(const Channel& other)
{
    if (this != &other) {
        // Delete the existing dynamic array.
        delete [] _clients;
        
        // Copy the basic members.
        _name = other._name;
        _clientCount = other._clientCount;
        _clientCapacity = other._clientCapacity;
        _operator = other._operator;

        // Allocate a new dynamic array and copy client pointers.
        _clients = new Client*[ _clientCapacity ];
        for (size_t i = 0; i < _clientCount; ++i) {
            _clients[i] = other._clients[i];
        }
    }
    return *this;
}

// Destructor: free the dynamic array.
// (Note: The Channel does not own the Client objects; it just stores pointers.)
Channel::~Channel()
{
    delete [] _clients;
}

const std::string& Channel::getName() const
{
    return _name;
}

// Expand the dynamic array capacity when needed.
void Channel::expandCapacity()
{
    size_t newCapacity = _clientCapacity * 2;
    Client** newClients = new Client*[newCapacity];

    // Copy existing client pointers.
    for (size_t i = 0; i < _clientCount; ++i) {
        newClients[i] = _clients[i];
    }

    // Delete the old array and update pointers and capacity.
    delete [] _clients;
    _clients = newClients;
    _clientCapacity = newCapacity;
}

// Add a client to the channel.
// The first client becomes the channel operator.
void Channel::addClient(Client* client)
{
    if (client == nullptr)
        return;

    // Expand capacity if we've reached the limit.
    if (_clientCount == _clientCapacity) {
        expandCapacity();
    }

    _clients[_clientCount] = client;
    _clientCount++;

    // If no operator is assigned, make this client the operator.
    if (_operator == nullptr)
        _operator = client;
}

// Remove a client from the channel.
// If the client is the operator, assign a new operator if possible.
void Channel::removeClient(Client* client)
{
    if (client == nullptr)
        return;

    // Locate the client in the array.
    size_t index = _clientCount; // Initialize with an invalid index.
    for (size_t i = 0; i < _clientCount; ++i) {
        if (_clients[i] == client) {
            index = i;
            break;
        }
    }
    // If the client is not found, do nothing.
    if (index == _clientCount)
        return;

    // Shift elements down to overwrite the removed client.
    for (size_t i = index; i < _clientCount - 1; ++i) {
        _clients[i] = _clients[i + 1];
    }
    _clientCount--;

    // If the removed client was the operator, reassign the operator.
    if (_operator == client) {
        _operator = (_clientCount > 0) ? _clients[0] : nullptr;
    }
}

bool Channel::isOperator(Client* client) const
{
    return client == _operator;
}

// KICK command:
// Only the operator (sender) can kick a target from the channel.
bool Channel::kick(Client* sender, Client* target)
{
    if (sender == nullptr || target == nullptr)
        return false;

    // Verify that the sender is the operator.
    if (!isOperator(sender))
        return false;

    // Ensure that the target is actually in the channel.
    bool found = false;
    for (size_t i = 0; i < _clientCount; ++i) {
        if (_clients[i] == target) {
            found = true;
            break;
        }
    }
    if (!found)
        return false;

    removeClient(target);
    std::cout << "Kicked " << target->getNickName() << " from " << _name << std::endl;
    return true;
}
