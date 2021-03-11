#include <mutex>

struct Node
{
    // Node struct contains a node value and a pointer to the next node
    int nodeValue;
    std::shared_ptr<Node> nextNode;
};

class SimpleQueue
{
public:
    // quick constructor to set head and tail to nullptr
    SimpleQueue()
        : head(nullptr),
          tail(nullptr)
    {
    }

    // enqueues an item in the queue, returns 0 if it went okay, -1 for a failure
    int enqueue(int item)
    {
        std::lock_guard<std::mutex> queueLock(queueMutex);
        std::shared_ptr<Node> newNode(new Node());
        newNode->nextNode = nullptr;
        newNode->nodeValue = item;

        // check for first item
        if (tail == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->nextNode = newNode;
            tail = newNode;
        }

        // always returning 0 (success) probably isn't desired, but I can't
        // think of any issues at the moment that would cause us to return -1
        // probably an exception I'm missing.
        return 0;
    }

    // dequeues an item from the queue. returns 0 if it went okay, -1 for failure
    // note: I implemented it where it would traverse the queue looking for a
    //      specific item to delete rather than deleting the first item enqueued
    //      into the queue (First In First Out)
    int dequeue(int *item)
    {
        std::lock_guard<std::mutex> queueLock(queueMutex);
        std::shared_ptr<Node> traverseNode(head);
        std::shared_ptr<Node> trailingNode(new Node());
        while (traverseNode != nullptr)
        {
            if (head == tail)
            {
                // only one item in the queue
                // delete it and set head and tail back to nullptr
                head = nullptr;
                tail = nullptr;
                return 0;
            }
            else if (traverseNode->nodeValue == *item)
            {
                if (traverseNode == head)
                {
                    // deleting the head so set head to the next node before
                    head = head->nextNode;
                }
                else if (traverseNode == tail)
                {
                    // deleting the tail so set the tail to our trailing node
                    // and set that node's next node to null
                    tail = trailingNode;
                    tail->nextNode = nullptr;
                }
                else
                {
                    // deleting in the middle so have to bridge the gap from the
                    // previous node to the next node before deleting
                    trailingNode->nextNode = traverseNode->nextNode;
                }

                // return success
                return 0;
            }
            trailingNode = traverseNode;
            traverseNode = traverseNode->nextNode;
        }

        // if we made it here that means we didn't delete anything so return
        // failure
        return -1;
    }

private:
    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    std::mutex queueMutex;
};

int main()
{
    return 0;
}