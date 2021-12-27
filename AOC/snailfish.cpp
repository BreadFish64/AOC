#include "types.hpp"

class TreeNode {
    TreeNode* parent = nullptr;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
    int number = 0;

    TreeNode* GetParent() const { return parent; }
    std::optional<int> GetNumber() const {
        return left ? std::nullopt : std::make_optional(number);
    }

    bool Split() {
        if (number < 10) return false;
        const int left_num = number / 2;
        const int right_num = number - left_num;
        left = std::make_unique<TreeNode>(this, left_num);
        right = std::make_unique<TreeNode>(this, number - left_num);
        number = 0;
        // Explode();
        return true;
    }

    bool Explode() {
        if (Depth() < 5) return false;
        if (!left->GetNumber() || !right->GetNumber()) return false;
        if (TreeNode* left_sum = left->Prev(); left_sum) left_sum->number += left->number;
        if (TreeNode* right_sum = right->Next(); right_sum) right_sum->number += right->number;
        left.reset();
        right.reset();
        return true;
    }

    void Reduce() {
        bool splitting;
        do {
            TreeNode* it = LeftNumber();
            while (it) {
                auto parent = it->parent;
                if (parent && parent->Explode()) {
                    it = parent;
                } else {
                    it = it->Next();
                }
            }
            splitting = false;
            it = LeftNumber();
            while (it) {
                if (it->Split()) {
                    splitting = true;
                    break;
                }
                it = it->Next();
            }
        } while (splitting);
    }

    usize Depth() const {
        const TreeNode* p = this;
        usize depth = 0;
        while (p) {
            ++depth;
            p = p->parent;
        }
        return depth;
    }

    bool IsLeft() const { return parent && this == parent->left.get(); }
    bool IsRight() const { return parent && this == parent->right.get(); }

    TreeNode* LeftNumber() {
        TreeNode* left = this;
        while (!left->GetNumber()) left = left->left.get();
        return left;
    }
    TreeNode* RightNumber() {
        TreeNode* right = this;
        while (!right->GetNumber()) right = right->right.get();
        return right;
    }

    TreeNode* Next() {
        TreeNode* next = this;
        while (!next->IsLeft()) {
            if (!next->parent) return nullptr;
            next = next->parent;
        }
        next = next->parent;
        if (!next) return nullptr;
        return next->right->LeftNumber();
    }
    TreeNode* Prev() {
        TreeNode* next = this;
        while (!next->IsRight()) {
            if (!next->parent) return nullptr;
            next = next->parent;
        }
        next = next->parent;
        if (!next) return nullptr;
        return next->left->RightNumber();
    }

public:
    TreeNode(TreeNode&& other) = delete;
    TreeNode(const TreeNode& other) {
        parent = other.parent;
        auto onumber = other.GetNumber();
        if (onumber) {
            number = *onumber;
        } else {
            left = std::make_unique<TreeNode>(*other.left);
            right = std::make_unique<TreeNode>(*other.right);
            left->parent = this;
            right->parent = this;
        }
    }
    TreeNode(std::unique_ptr<TreeNode>&& _left, std::unique_ptr<TreeNode>&& _right)
        : left{std::move(_left)}, right{std::move(_right)} {
        left->parent = this;
        right->parent = this;
        Reduce();
    }
    TreeNode(TreeNode* parent, int number) : parent{parent}, number{number} {}
    TreeNode(TreeNode* parent, std::istream& stream) : parent{parent} {
        char c{};
        stream >> c;
        if (c == '[') {
            left.reset(new TreeNode(this, stream));
            stream.ignore(1); // ','
            right.reset(new TreeNode(this, stream));
            stream.ignore(1); // ']'
        } else {
            number = c - '0';
        }
    }

    u64 Magnitude() const {
        auto number = GetNumber();
        if (number) return *number;
        return 3 * left->Magnitude() + 2 * right->Magnitude();
    }

    operator std::string() const {
        auto number = GetNumber();
        if (number) {
            return fmt::format("{}", *number);
        } else {
            return fmt::format("[{},{}]", static_cast<std::string>(*left),
                               static_cast<std::string>(*right));
        }
    }
};

int main() {
    std::ifstream input_file{"input.txt"};
    std::istream& input = input_file;
    std::vector<std::unique_ptr<TreeNode>> trees;
    while (!input.eof()) {
        trees.emplace_back(std::make_unique<TreeNode>(nullptr, input));
        input.ignore(1); // '\n'
    }

    std::unique_ptr<TreeNode> sum = std::make_unique<TreeNode>(*trees.front());
    for (auto root = trees.begin() + 1; root != trees.end(); ++root) {
        sum = std::make_unique<TreeNode>(std::move(sum), std::make_unique<TreeNode>(**root));
    }
    fmt::print("{}\n", static_cast<std::string>(*sum));
    fmt::print("P1: {}\n", sum->Magnitude());

    u64 max = 0;
    for (auto aroot = trees.begin(); aroot != trees.end(); ++aroot) {
        for (auto broot = trees.begin(); broot != trees.end(); ++broot) {
            if (aroot == broot) continue;
            auto sum = std::make_unique<TreeNode>(std::make_unique<TreeNode>(**aroot),
                                                  std::make_unique<TreeNode>(**broot));
            u64 magnitude = sum->Magnitude();
            if (magnitude > max) max = magnitude;
        }
    }
    fmt::print("P2: {}\n", max);
}
