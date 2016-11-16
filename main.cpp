#include <iostream>
#include <memory>

struct TreeNode {
  int val;
  std::shared_ptr<TreeNode> left, right;
  TreeNode(int v, const std::shared_ptr<TreeNode> & lchild = nullptr, const std::shared_ptr<TreeNode> & rchild = nullptr)
    : val(v), left(lchild), right(rchild) {}
};

using pNode = std::shared_ptr<TreeNode>;

class solution {
public:
  int maxPathSum(const pNode & root) {
    
  }
};

int main() {
  
}
