#ifndef UNROLLEDLINKEDLIST_H
#define UNROLLEDLINKEDLIST_H

template<class key_type, class value_type, size_t max_size = 100000, size_t max_block_size = 370>
class UnrolledLinkedList {
private:
  struct HeadNode {
    key_type first_key_;
    value_type first_val_;
    key_type last_key_;
    value_type last_val_;
    size_t nxt_;
    size_t block_size_;
  };
  using BodyNode = std::array<std::pair<key_type, value_type>, max_block_size>;
  MemoryRiver<HeadNode, 3> head_file_;
  MemoryRiver<BodyNode, 1> body_file_;
  size_t size_of_head_node_ = sizeof(HeadNode);
  size_t size_of_body_node_ = sizeof(BodyNode);
  size_t cur_size_;

  struct CacheHeadNode {
    HeadNode node_;
    size_t pos_;
    CacheHeadNode *nxt_;
    CacheHeadNode() = delete;
    CacheHeadNode(HeadNode node, size_t pos, CacheHeadNode* nxt = nullptr) : node_(node), pos_(pos), nxt_(nxt) {}
  };
  CacheHeadNode *cache_head_;

  void BlockInsert(CacheHeadNode *,const key_type &, const value_type &);
  void BlockFind(std::vector<value_type> &, CacheHeadNode *, const key_type &);
  bool BlockDelete(CacheHeadNode *, const std::pair<key_type, value_type> &);
  void BlockPrint(CacheHeadNode *);
public:
  UnrolledLinkedList() = delete;
  UnrolledLinkedList(const std::string &);
  ~UnrolledLinkedList();
  void Insert(const key_type &, const value_type &);
  std::vector<value_type> Find(const key_type &);
  bool Delete(const key_type &, const value_type &);
  void Print();
};

#endif //UNROLLEDLINKEDLIST_H
