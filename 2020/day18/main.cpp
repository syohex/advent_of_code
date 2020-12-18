#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

namespace {

enum class Type {
    kNumber,
    kAdd,
    kMul,
    kEOF,
    kLparen,
    kRparen,
};

struct Token {
    Type type;
    std::int64_t number;

    void Dump() const {
        switch (type) {
        case Type::kNumber:
            std::cout << "Number: " << number << std::endl;
            break;
        case Type::kAdd:
            std::cout << "Opcode: Add" << std::endl;
            break;
        case Type::kMul:
            std::cout << "Opcode: Mul" << std::endl;
            break;
        case Type::kLparen:
            std::cout << "Left paren" << std::endl;
            break;
        case Type::kRparen:
            std::cout << "Right paren" << std::endl;
            break;
        case Type::kEOF:
            std::cout << "EOF" << std::endl;
            break;
        default:
            assert(!"never reach here");
            break;
        }
    }

    bool operator==(const Token &other) const {
        if (type != other.type) {
            return false;
        }

        if (type != Type::kNumber) {
            return true;
        }

        return number == other.number;
    }
};

struct Tokenizer {
    explicit Tokenizer(const std::string &input) : input(input), pos(0) {
        limit = input.size();
    }

    Token NextToken() {
        while (pos < limit && input[pos] == ' ') {
            ++pos;
        }

        if (pos == limit) {
            return Token{Type::kEOF};
        }

        if (input[pos] == '+') {
            ++pos;
            return Token{Type::kAdd};
        } else if (input[pos] == '*') {
            ++pos;
            return Token{Type::kMul};
        } else if (input[pos] == '(') {
            ++pos;
            return Token{Type::kLparen};
        } else if (input[pos] == ')') {
            ++pos;
            return Token{Type::kRparen};
        }

        std::int64_t ret = 0;
        while (pos < limit && input[pos] >= '0' && input[pos] <= '9') {
            ret = ret * 10 + input[pos] - '0';
            ++pos;
        }

        return Token{Type::kNumber, ret};
    }

    std::vector<Token> Tokenize() {
        std::vector<Token> tokens;
        while (true) {
            auto token = NextToken();
            if (token.type == Type::kEOF) {
                break;
            }

            tokens.emplace_back(std::move(token));
        }

        return tokens;
    }

    const std::string &input;
    size_t pos;
    size_t limit;
};

struct Parser01 {
    explicit Parser01(const std::vector<Token> &tokens) : tokens(tokens), pos(0) {
    }

    std::int64_t Eval() {
        return Expression();
    }

    std::int64_t Expression() {
        std::int64_t ret = Term();
        while (pos < tokens.size() && (Peek().type == Type::kAdd || Peek().type == Type::kMul)) {
            Token op = NextToken();

            if (op.type == Type::kAdd) {
                ret += Term();
            } else if (op.type == Type::kMul) {
                ret *= Term();
            }
        }

        return ret;
    }

    std::int64_t Term() {
        Token token = NextToken();
        if (token.type == Type::kNumber) {
            return token.number;
        } else if (token.type == Type::kLparen) {
            std::int64_t val = Expression();
            Token t = NextToken();
            assert(t.type == Type::kRparen);
            return val;
        }

        assert(!"never reach here");
        return -1;
    }

    Token NextToken() {
        if (pos >= tokens.size()) {
            return Token{Type::kEOF};
        }
        return tokens[pos++];
    }

    Token Peek() {
        return tokens[pos];
    }

    const std::vector<Token> &tokens;
    size_t pos;
};

struct Parser02 {
    explicit Parser02(const std::vector<Token> &tokens) : tokens(tokens), pos(0) {
    }

    std::int64_t Eval() {
        return Expression();
    }

    std::int64_t Expression() {
        std::int64_t ret = Term();
        while (pos < tokens.size() && Peek().type == Type::kMul) {
            ++pos; // skip opcode

            ret *= Term();
        }

        return ret;
    }

    std::int64_t Term() {
        auto ret = Factor();
        while (pos < tokens.size() && Peek().type == Type::kAdd) {
            ++pos; // skip opcode

            ret += Factor();
        }

        return ret;
    }

    std::int64_t Factor() {
        const auto &token = Peek();
        if (token.type == Type::kNumber) {
            ++pos;
            return token.number;
        } else if (token.type == Type::kLparen) {
            ++pos; // skip left paren
            std::int64_t value = Expression();
            ++pos; // skip right paren

            return value;
        }

        assert(!"never reach here in factor");
        return 0;
    }

    Token NextToken() {
        if (pos >= tokens.size()) {
            return Token{Type::kEOF};
        }
        return tokens[pos++];
    }

    Token Peek() {
        return tokens[pos];
    }

    const std::vector<Token> &tokens;
    size_t pos;
};

std::int64_t Solve01(const std::vector<std::string> &inputs) {
    std::int64_t ret = 0;

    for (const auto &input : inputs) {
        auto tokens = Tokenizer(input).Tokenize();
        Parser01 parser(tokens);
        ret += parser.Eval();
    }

    return ret;
}

std::int64_t Solve02(const std::vector<std::string> &inputs) {
    std::int64_t ret = 0;

    for (const auto &input : inputs) {
        auto tokens = Tokenizer(input).Tokenize();
        Parser02 parser(tokens);
        ret += parser.Eval();
    }

    return ret;
}

void Test01() {
    {
        std::string input("1 + (2 * 3) + (4 * (5 + 6))");
        Tokenizer tokenizer(input);
        auto tokens = tokenizer.Tokenize();
        assert((tokens[0] == Token{Type::kNumber, 1}));
        assert((tokens[1] == Token{Type::kAdd}));
        assert((tokens[2] == Token{Type::kLparen}));
        assert((tokens[3] == Token{Type::kNumber, 2}));
        assert((tokens[4] == Token{Type::kMul}));
        assert((tokens[5] == Token{Type::kNumber, 3}));
        assert((tokens[6] == Token{Type::kRparen}));
        assert((tokens[7] == Token{Type::kAdd}));
        assert((tokens[8] == Token{Type::kLparen}));
        assert((tokens[9] == Token{Type::kNumber, 4}));
        assert((tokens[10] == Token{Type::kMul}));
        assert((tokens[11] == Token{Type::kLparen}));
        assert((tokens[12] == Token{Type::kNumber, 5}));
        assert((tokens[13] == Token{Type::kAdd}));
        assert((tokens[14] == Token{Type::kNumber, 6}));
        assert((tokens[15] == Token{Type::kRparen}));
        assert((tokens[16] == Token{Type::kRparen}));

        Parser01 parser(tokens);
        auto ret = parser.Eval();
        assert(ret == 51);
    }

    {
        std::string input("1 + 2 * 3 + 4 * 5 + 6");
        Tokenizer tokenizer(input);
        auto tokens = tokenizer.Tokenize();
        Parser01 parser(tokens);
        auto ret = parser.Eval();
        assert(ret == 71);
    }

    // clang-format off
    std::vector<std::string> inputs {
        "2 * 3 + (4 * 5)",
        "5 + (8 * 3 + 9 + 3 * 4 * 3)",
        "5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))",
        "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2",
    };
    // clang-format on

    assert(Solve01(inputs) == 26335);
}

void Test02() {
    {
        std::string input("1 + 2 * 3 + 4 * 5 + 6");
        Tokenizer tokenizer(input);
        auto tokens = tokenizer.Tokenize();
        Parser02 parser(tokens);
        auto ret = parser.Eval();
        assert(ret == 231);
    }

    // clang-format off
    std::vector<std::string> inputs {
        "1 + (2 * 3) + (4 * (5 + 6))",
        "2 * 3 + (4 * 5)",
        "5 + (8 * 3 + 9 + 3 * 4 * 3)",
        "5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))",
        "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2 ",
    };
    // clang-format on

    assert(Solve02(inputs) == 693942);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::vector<std::string> inputs;
    std::string line;
    while (std::getline(std::cin, line)) {
        inputs.push_back(line);
    }

    std::cout << "Part01: " << Solve01(inputs) << std::endl;
    std::cout << "Part02: " << Solve02(inputs) << std::endl;
    return 0;
}
