#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <memory>

enum class DataType {
    kNum,
    kList,
};

enum class Result {
    kWin,
    kDraw,
    kLose,
};

struct Element {
    Element(DataType type) : type_(type) {
    }
    virtual ~Element() = default;

    DataType Type() const noexcept {
        return type_;
    }

    virtual Result Compare(const Element *right) const = 0;

    virtual int Value() const {
        throw "never reach here";
    }

    virtual const std::vector<Element *> &Values() const {
        throw "never reach here";
    }

    DataType type_;
};

struct ValueElement : public Element {
    explicit ValueElement(int value) : Element(DataType::kNum), value_(value) {
    }

    Result Compare(const Element *right) const override;

    int Value() const override {
        return value_;
    }

    int value_;
};

struct ListElement : public Element {
    ListElement() : Element(DataType::kList) {
    }
    ~ListElement() {
        for (Element *e : values_) {
            delete e;
        }
    }

    void AddElement(Element *e) {
        values_.push_back(e);
    }

    Result Compare(const Element *right) const override;

    const std::vector<Element *> &Values() const override {
        return values_;
    }

    std::vector<Element *> values_;
};

Result ValueElement::Compare(const Element *right) const {
    if (right->Type() == DataType::kNum) {
        if (value_ < right->Value()) {
            return Result::kWin;
        }
        if (value_ == right->Value()) {
            return Result::kDraw;
        }

        return Result::kLose;
    }

    auto left = std::make_unique<ListElement>();
    left->AddElement(new ValueElement(value_));
    return left->Compare(right);
}

Result ListElement::Compare(const Element *right) const {
    if (right->Type() == DataType::kNum) {
        auto right_list = std::make_unique<ListElement>();
        right_list->AddElement(new ValueElement(right->Value()));
        return Compare(right_list.get());
    }

    const std::vector<Element *> right_values = right->Values();
    size_t left_len = values_.size();
    size_t right_len = right_values.size();
    size_t len = std::min(left_len, right_len);

    for (size_t i = 0; i < len; ++i) {
        Result result = values_[i]->Compare(right_values[i]);
        switch (result) {
        case Result::kDraw:
            break;
        default:
            return result;
        }
    }

    if (left_len < right_len) {
        return Result::kWin;
    }
    if (left_len == right_len) {
        return Result::kDraw;
    }

    return Result::kLose;
}

Element *ParseLine(const std::string &s, size_t &pos, ListElement *current) {
    size_t len = s.size();
    while (pos < len) {
        switch (s[pos]) {
        case '[': {
            ++pos;

            ListElement *list = new ListElement();
            if (current == nullptr) {
                return ParseLine(s, pos, list);
            }

            Element *e = ParseLine(s, pos, list);
            current->AddElement(e);
            break;
        }
        case ']':
            ++pos;
            return current;
        case ',':
        case ' ':
            ++pos;
            break;
        default: {
            assert(s[pos] >= '0' && s[pos] <= '9');

            int num = s[pos] - '0';
            ++pos;
            while (pos < len && s[pos] >= '0' && s[pos] <= '9') {
                num = num * 10 + s[pos] - '0';
                ++pos;
            }

            current->AddElement(new ValueElement(num));
            break;
        }
        }
    }

    // never reach here
    assert(false);
    return nullptr;
}

template <typename T>
std::vector<std::pair<Element *, Element *>> ParseInput(T &it) {
    std::vector<std::pair<Element *, Element *>> ret;
    std::string line;
    while (std::getline(it, line)) {
        size_t p1 = 0;
        Element *e1 = ParseLine(line, p1, nullptr);

        std::getline(it, line);
        size_t p2 = 0;
        Element *e2 = ParseLine(line, p2, nullptr);

        ret.push_back({e1, e2});

        // skip empty line
        std::getline(it, line);
    }

    return ret;
}

int Problem1(const std::vector<std::pair<Element *, Element *>> &data) {
    int ret = 0;
    int i = 1;
    for (const auto &p : data) {
        const Element *e1 = p.first;
        const Element *e2 = p.second;

        if (e1->Compare(e2) == Result::kWin) {
            ret += i;
        }

        ++i;
    }

    return ret;
}

void Test() {
    std::ifstream fs("test.txt");
    auto data = ParseInput(fs);

    int ret1 = Problem1(data);
    assert(ret1 == 13);

    for (auto &p : data) {
        delete p.first;
        delete p.second;
    }
}

int main() {
    Test();

    std::ifstream fs("input.txt");
    auto data = ParseInput(fs);
    int ret1 = Problem1(data);

    std::cout << "Problem1: " << ret1 << std::endl;

    for (auto &p : data) {
        delete p.first;
        delete p.second;
    }
    return 0;
}
