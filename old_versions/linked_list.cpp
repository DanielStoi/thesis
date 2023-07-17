


template <typename T>
struct linked_list{
public:
    linked_node<T>* head = NULL;
    linked_node<T>* tail = NULL;
    int id;
    int size = 0;

    linked_list(int id){
        this.id = id;
    }

    linked_list(){
        this.id = 0;
    }

    push_back(T val){
        linked_node<T>* new_node = new linked_node<T>(this, val);
        size++;
        if (size == 0){
            head = new_node;
            tail = new_node;
            return;
        }
        tail->next = new_node;
        tail = new_node;
    }

    void insert_before(T val, linked_node<T>* node){
        linked_node<T>* new_node = new linked_node<T>(this, val);
        size++;
        linked_node<T>* prev = node->prev;

        if (prev)
            prev->next = new_node;
        else
            head = node;
        
        new_node->prev = prev;
        new_node->next = node;
        node->prev = new_node;

    }
};


template <typename T>
struct linked_node{
public:
    linked_node<T>* next = NULL;
    linked_node<T>* prev = NULL;
    linked_list<T>* list;
    T val;

    linked_node(linked_list<T>* list, T val){
        this.list = list;
        this.val = val;
    }
} ;


template <typename T>
void remove_node(linked_node<T>* node){
    if (node == NULL){
        return;
    }
    if (node->prev != NULL){
        node->prev->next = node->next;
    }
    if (node->next != NULL){
        node->next->prev = node->prev;
    }
    node->list.size--;
    delete(node);
};