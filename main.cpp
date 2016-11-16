#include <iostream>
#include <memory>
#include <vector>
#include <iterator>
#include <numeric>
#include <queue>
#include <algorithm>

struct TreeNode {
    int val;
    std::shared_ptr<TreeNode> left, right;
    TreeNode(int v, const std::shared_ptr<TreeNode> & lchild = nullptr, const std::shared_ptr<TreeNode> & rchild = nullptr)
        : val(v), left(lchild), right(rchild) {}

    void print() {
        if (left != nullptr || right != nullptr) {
            std::cout << "val = " << val
                      << "\t -> left = " << (left != nullptr ? std::to_string(left->val) : "null") << std::endl
                      << "\t -> right = " << (right != nullptr ? std::to_string(right->val) : "null") << std::endl;
        }
    }
    
    std::ostream & output(std::ostream & os) {
        os << val;
        return os;
    }
};

using pNode = std::shared_ptr<TreeNode>;

std::ostream & operator<<(std::ostream & os, const pNode & p) {
    if (p != nullptr) {
        p->output(os);
    }
    return os;
}

pNode buildTree(const std::vector<int> & nums) {
    pNode root = nullptr;
    if (nums.empty())
        return root;
    if (nums.size() == 1)
        root = std::make_shared<TreeNode>(nums[0]);
    else {
        size_t mid = nums.size()/2;
        root = std::make_shared<TreeNode>(nums[mid]);
        root->left = buildTree(std::vector<int>(nums.begin(), nums.begin()+mid));
        root->right = buildTree(std::vector<int>(nums.begin()+mid+1, nums.end()));
    }
    return root;
}

void printTree(const pNode & node) {
    if (node != nullptr) {
        printTree(node->left);
        std::cout << node->val << " ";
        printTree(node->right);
    }
}

void printBFS(const pNode & root) {
    std::queue<pNode> qu;
    qu.push(root);
    while (!qu.empty()) {
        auto curr = qu.front();
        qu.pop();
        if (curr != nullptr) {
            curr->print();
            qu.push(curr->left);
            qu.push(curr->right);
        }
    }
}

namespace std {
    template<>
    struct less<std::vector<pNode>> {
        bool operator()(const std::vector<pNode> & v1, const std::vector<pNode> & v2) {
            return std::accumulate(v1.begin(), v1.end(), 0, [](int i, const pNode & p){
                return p == nullptr ? i : i+p->val; 
            }) < std::accumulate(v2.begin(), v2.end(), 0, [](int i, const pNode & p){
                return p == nullptr ? i : i+p->val;
            });
        }
    };
}

class solution {
  public:
    int maxPathSum(const pNode & root) {
        if (root != nullptr) {
            std::vector<std::vector<pNode>> leftpaths = getPaths(std::vector<pNode>{root->left});
            auto leftit = std::max_element(leftpaths.begin(), leftpaths.end(), std::less<std::vector<pNode>>());
            // std::copy(leftit->begin(), leftit->end(), std::ostream_iterator<pNode>(std::cout, " "));
            // std::cout << std::endl;
        
            auto rightpaths = getPaths(std::vector<pNode>{root->right});
            auto rightit = std::max_element(rightpaths.begin(), rightpaths.end(), std::less<std::vector<pNode>>());
            // std::copy(rightit->begin(), rightit->end(), std::ostream_iterator<pNode>(std::cout, " "));
            // std::cout << std::endl;

            int maxPSum = root->val + std::accumulate(leftit->begin(), leftit->end(), 0,
                                                      [](int i, const pNode & p){
                                                          return p == nullptr ? i : i+p->val;
                                                      })
                + std::accumulate(rightit->begin(), rightit->end(), 0,
                                  [](int i, const pNode & p){
                                      return p == nullptr ? i : i+p->val;
                                  });
        
            return std::max({maxPathSum(root->left), maxPathSum(root->right), maxPSum});
        }
        return 0;
    }

  private:
    std::vector<std::vector<pNode>> getPaths(const std::vector<pNode> & curr) {
        std::vector<std::vector<pNode>> result;
        if (curr.back() != nullptr) {
            if (curr.back()->left != nullptr || curr.back()->right != nullptr) {
                if (curr.back()->left != nullptr) {
                    auto cpy = curr;
                    cpy.push_back(curr.back()->left);
                    auto temp = getPaths(cpy);
                    std::copy(temp.begin(), temp.end(), std::inserter(result, result.end()));
                }
                if (curr.back()->right != nullptr) {
                    auto cpy = curr;
                    cpy.push_back(curr.back()->right);
                    auto temp = getPaths(cpy);
                    std::copy(temp.begin(), temp.end(), std::inserter(result, result.end()));
                }
            }
            else {
                result.push_back(curr);
            }
        }
        else {
            result.push_back(curr);
        }
        return result;
    }
};

int main() {
    std::vector<int> nums{2,1,3};
    pNode root = buildTree(nums);
    root->right->left = std::make_shared<TreeNode>(4);
    root->right->right = std::make_shared<TreeNode>(5);
    printBFS(root);

    solution soln;
    int mPathSum = soln.maxPathSum(root);
    std::cout << "The maximum path sum is:\n"
              << mPathSum << std::endl;
}
