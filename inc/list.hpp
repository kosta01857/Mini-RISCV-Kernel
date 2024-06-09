//
// Created by kosta on 6.8.23..
//

#ifndef PROJ_BASE_LIST_HPP
#define PROJ_BASE_LIST_HPP
template<typename T>

class List
{
public:
    struct Node
    {
        T *data;
        Node *next;
        Node(T *data, Node *next) : data(data), next(next) {}
    };
    List() {
        head = nullptr;
        tail = nullptr;
    }
    List(const List<T> &) = delete;
    void addFirst(T *data)
    {
        Node *elem = new Node(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }
    void addLast(T *data)
    {
        Node *elem = new Node(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }
    T *removeFirst()
    {
        if (!head) { return 0; }
        Node *elem = head;
        head = head->next;
        if (!head) { tail = 0; }
        T *ret = elem->data;
        delete elem;
        return ret;
    }
    Node* get_head() const{
        return head;
    }
    ~List(){
        while(head != nullptr){
            Node* next = head->next;
            delete head;
            head = next;
        }
    }
private:
    Node *head, *tail;
};
#endif //PROJ_BASE_LIST_HPP
