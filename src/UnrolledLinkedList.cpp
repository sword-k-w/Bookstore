//
// Created by sword on 2024/12/7.
//

#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <cstring>
#include "MemoryRiver.hpp"
#include "UnrolledLinkedList.h"

template<class key_type, class value_type, size_t max_size, size_t max_block_size>
UnrolledLinkedList<key_type, value_type, max_size, max_block_size>::UnrolledLinkedList(const std::string &file_name) : head_file_(file_name + "_Head"), body_file_(file_name + "_Body") {
#ifdef debug
  std::cerr << "size of head node : " << size_of_head_node_ << "\n";
  std::cerr << "size of body node : " << size_of_body_node_ << "\n";
#endif

  head_file_.initialise();
  body_file_.initialise();
  head_file_.get_info(cur_size_, 2);

  if (cur_size_) {
    HeadNode cur_head;
    size_t head_pos;
    head_file_.get_info(head_pos, 1);
    head_file_.read(cur_head, head_pos);
    cache_head_ = new CacheHeadNode(cur_head, head_pos);
    CacheHeadNode *tmp = cache_head_;
    while (true) {
      if (!cur_head.nxt_) {
        break;
      }
      head_pos = cur_head.nxt_;
      head_file_.read(cur_head, head_pos);
      tmp->nxt_ = new CacheHeadNode(cur_head, head_pos);
      tmp = tmp->nxt_;
    }
  }
}

template<class key_type, class value_type, size_t max_size, size_t max_block_size>
UnrolledLinkedList<key_type, value_type, max_size, max_block_size>::~UnrolledLinkedList() {
  head_file_.write_info(cur_size_, 2);
  if (cur_size_) {
    head_file_.write_info(cache_head_->pos_, 1);
    while (true) {
      head_file_.update(cache_head_->node_, cache_head_->pos_);
      CacheHeadNode *tmp = cache_head_->nxt_;
      delete cache_head_;
      if (tmp == nullptr) {
        break;
      }
      cache_head_ = tmp;
    }
  }
}

template<class key_type, class value_type, size_t max_size, size_t max_block_size>
void UnrolledLinkedList<key_type, value_type, max_size, max_block_size>::Insert(const key_type &key, const value_type &val) {
  if (cur_size_ == 0) {
    HeadNode head = {key, val, key, val, 0, 1};

    size_t head_pos = head_file_.reserve();
    cache_head_ = new CacheHeadNode(head, head_pos);

    BodyNode body = {std::make_pair(key, val)};
    body_file_.update(body, head_pos);
  } else {
    CacheHeadNode *tmp = cache_head_;
    while (true) {
      if (tmp->nxt_ == nullptr) {
        BlockInsert(tmp, key, val);
        break;
      }
      if (key < tmp->nxt_->node_.first_key_ || (key == tmp->nxt_->node_.first_key_ && val < tmp->nxt_->node_.first_val_)) {
        BlockInsert(tmp, key, val);
        break;
      }
      tmp = tmp->nxt_;
    }
  }
  ++cur_size_;
}

template<class key_type, class value_type, size_t max_size, size_t max_block_size>
void UnrolledLinkedList<key_type, value_type, max_size, max_block_size>::BlockInsert(CacheHeadNode *head_node, const key_type &key, const value_type &val) {
  BodyNode tmp_array;
  body_file_.read(tmp_array, head_node->pos_);
  std::pair<key_type, value_type> tmp_node = std::make_pair(key, val);
  size_t tmp_size = head_node->node_.block_size_;
  if (!tmp_size) {
    tmp_array = {tmp_node};
  } else {
    size_t pos = 0;
    if (tmp_node > tmp_array[0]) {
      for (size_t i = 0; i < tmp_size; ++i) {
        if (i + 1 == tmp_size || tmp_node < tmp_array[i + 1]) {
          pos = i + 1;
          break;
        }
      }
    }
    for (size_t i = tmp_size; i > pos; --i) {
      tmp_array[i] = tmp_array[i - 1];
    }
    tmp_array[pos] = tmp_node;
  }
  ++tmp_size;

  // for (size_t i = 0; i < tmp_size; ++i) {
  //   std::cerr << tmp_array[i].first[0] << " ";
  // }
  // std::cerr << '\n';

  if (tmp_size == max_block_size) { // Split block
    size_t split_pos = tmp_size / 2;

    CacheHeadNode *new_node = new CacheHeadNode({tmp_array[split_pos].first, tmp_array[split_pos].second, tmp_array[tmp_size - 1].first, tmp_array[tmp_size - 1].second, head_node->node_.nxt_, tmp_size - split_pos}, head_file_.reserve(), head_node->nxt_);

    BodyNode tmp_body;
    std::copy(tmp_array.begin() + split_pos, tmp_array.begin() + tmp_size, tmp_body.begin());
    body_file_.update(tmp_body, new_node->pos_);

    head_node->node_ = {tmp_array[0].first, tmp_array[0].second, tmp_array[split_pos - 1].first, tmp_array[split_pos - 1].second, new_node->pos_, split_pos};
    head_node->nxt_ = new_node;

    std::copy(tmp_array.begin(), tmp_array.begin() + split_pos, tmp_body.begin());
    body_file_.update(tmp_body, head_node->pos_);
  } else {
    body_file_.update(tmp_array, head_node->pos_);
    head_node->node_ = {tmp_array[0].first, tmp_array[0].second, tmp_array[tmp_size - 1].first, tmp_array[tmp_size - 1].second, head_node->node_.nxt_, tmp_size};
  }
}

template<class key_type, class value_type, size_t max_size, size_t max_block_size>
std::vector<value_type> UnrolledLinkedList<key_type, value_type, max_size, max_block_size>::Find(const key_type &key) {
  if (cur_size_ == 0) {
    return {};
  }
  CacheHeadNode *tmp = cache_head_;

  std::vector<value_type> res;
  while (true) {
    if (tmp->node_.first_key_ <= key && key <= tmp->node_.last_key_) {
      BlockFind(res, tmp, key);
    }
    if (tmp->nxt_ == nullptr || tmp->nxt_->node_.first_key_ > key) {
      break;
    }
    tmp = tmp->nxt_;
  }
  return res;
}

template<class key_type, class value_type, size_t max_size, size_t max_block_size>
void UnrolledLinkedList<key_type, value_type, max_size, max_block_size>::BlockFind(std::vector<value_type> &res, CacheHeadNode *head_node, const key_type &key) {
  BodyNode tmp_array;
  body_file_.read(tmp_array, head_node->pos_);
  for (size_t i = 0; i < head_node->node_.block_size_; ++i) {
    if (tmp_array[i].first == key) {
      res.emplace_back(tmp_array[i].second);
    }
  }
}

/**
 * @return whether (key, val) exists
 */
template<class key_type, class value_type, size_t max_size, size_t max_block_size>
bool UnrolledLinkedList<key_type, value_type, max_size, max_block_size>::Delete(const key_type &key, const value_type &val) {
  if (cur_size_ == 0) {
    return false;
  }
  CacheHeadNode *tmp = cache_head_;

  std::pair<key_type, value_type> key_val(key, val);
  while (true) {
    if (std::make_pair(tmp->node_.first_key_, tmp->node_.first_val_) <= key_val && key_val <= std::make_pair(tmp->node_.last_key_, tmp->node_.last_val_)) {
      bool res = BlockDelete(tmp, key_val);
      if (res) {
        --cur_size_;
      }
      return res;
    }
    if (tmp->nxt_ == nullptr || tmp->node_.first_key_ > key) {
      break;
    }
    tmp = tmp->nxt_;
  }
  return false;
}

/**
 * @return whether (key, val) exists
 */
template<class key_type, class value_type, size_t max_size, size_t max_block_size>
bool UnrolledLinkedList<key_type, value_type, max_size, max_block_size>::BlockDelete(CacheHeadNode *head_node, const std::pair<key_type, value_type> &key_val) {
  BodyNode tmp_array;
  body_file_.read(tmp_array, head_node->pos_);

  size_t tmp_size = head_node->node_.block_size_;
  size_t pos = tmp_size;
  for (size_t i = 0; i < tmp_size; ++i) {
    if (tmp_array[i] == key_val) {
      pos = i;
      break;
    }
  }
  if (pos == tmp_size) {
    return false;
  }
  for (size_t i = pos + 1; i < tmp_size; ++i) {
    tmp_array[i - 1] = tmp_array[i];
  }
  --tmp_size;

  // if (cur_head.nxt_) {
  //   HeadNode nxt_head;
  //   head_file_.read(nxt_head, cur_head.nxt_);
  //   if (tmp_size + nxt_head.block_size_ < 3 * max_block_size / 4) {
  //     body_pos = cur_head.nxt_ * max_block_size;
  //     tmp_array.resize(tmp_size + nxt_head.block_size_);
  //     for (size_t i = 0; i < nxt_head.block_size_; ++i) {
  //       body_file_.read(tmp_array[i + tmp_size], body_pos + i);
  //     }
  //     body_pos = head_pos * max_block_size;
  //     tmp_size += nxt_head.block_size_;
  //     for (size_t i = 0; i < tmp_size; ++i) {
  //       body_file_.update(tmp_array[i], body_pos + i);
  //     }
  //     HeadNode tmp = {tmp_array[0].key_, tmp_array[0].val_, tmp_array[tmp_size - 1].key_, tmp_array[tmp_size - 1].val_, nxt_head.nxt_, tmp_size};
  //     head_file_.update(tmp, head_pos);
  //     return true;
  //   }
  // }

  body_file_.update(tmp_array, head_node->pos_);

  head_node->node_ = {tmp_array[0].first, tmp_array[0].second, tmp_array[tmp_size - 1].first, tmp_array[tmp_size - 1].second, head_node->node_.nxt_, tmp_size};
  return true;
}

template<class key_type, class value_type, size_t max_size, size_t max_block_size>
void UnrolledLinkedList<key_type, value_type, max_size, max_block_size>::Print() {
  if (cur_size_ == 0) {
    return;
  }
  CacheHeadNode *tmp = cache_head_;

  std::vector<value_type> res;
  while (true) {
    BlockPrint(tmp);
    if (tmp->nxt_ = nullptr) {
      break;
    }
    tmp = tmp->nxt_;
  }
  std::cerr << '\n';
}

template<class key_type, class value_type, size_t max_size, size_t max_block_size>
void UnrolledLinkedList<key_type, value_type, max_size, max_block_size>::BlockPrint(CacheHeadNode * head_node) {
  BodyNode tmp_array;
  body_file_.read(tmp_array, head_node->pos_);
  for (size_t i = 0; i < head_node->node_.block_size_; ++i) {
    std::cerr << "(" << tmp_array[i].first << " " << tmp_array[i].second << ") ";
  }
  std::cerr << "\n";
}

std::array<char, 65> Transform(const char *x) {
  std::array<char, 65> res;
  size_t len = strlen(x);
  for (size_t i = 0; i < len; ++i) {
    res[i] = x[i];
  }
  for (size_t i = len; i < 65; ++i) {
    res[i] = 0;
  }
  return res;
}

std::ostream &operator << (std::ostream &os, const std::array<char, 65> x) {
  for (size_t i = 0; i < 65; ++i) {
    if (x[i]) {
      os << x[i];
    }
  }
  return os;
}