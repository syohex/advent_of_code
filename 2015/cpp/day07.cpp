#include <cassert>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <map>

using namespace std;

enum class Op {
    Assignment,
    BitwiseAnd,
    BitwiseOr,
    BitwiseNot,
    LeftShift,
    RightShift,
};

struct Expression {
    Op op;
    string src1;
    string src2;
    string dest;
};

auto IsNumber(const string &s) -> bool {
    return isdigit(s[0]);
}

auto ParseBinaryOp(Op op, const vector<string> &v) -> Expression {
    return {.op = op, .src1 = v[0], .src2 = v[2], .dest = v[4]};
}

auto Parse(const string &input) -> Expression {
    stringstream ss(input);
    vector<string> v;
    string tmp;

    while (getline(ss, tmp, ' ')) {
        v.push_back(tmp);
    }

    if (v[1] == "->") {
        return {.op = Op::Assignment, .src1 = v[0], .dest = v[2]};
    }

    if (v[0] == "NOT") {
        return {.op = Op::BitwiseNot, .src1 = v[1], .dest = v[3]};
    }

    if (v[1] == "AND") {
        return ParseBinaryOp(Op::BitwiseAnd, v);
    }
    if (v[1] == "OR") {
        return ParseBinaryOp(Op::BitwiseOr, v);
    }
    if (v[1] == "LSHIFT") {
        return ParseBinaryOp(Op::LeftShift, v);
    }
    if (v[1] == "RSHIFT") {
        return ParseBinaryOp(Op::RightShift, v);
    }

    assert(!"never reach here");
    return {};
}

auto CanEvaluateTerm(const string &term, const map<string, int> &env) -> bool {
    return IsNumber(term) || env.contains(term);
}

auto CanEvaluate(const Expression &exp, const map<string, int> &env) -> bool {
    switch (exp.op) {
    case Op::Assignment:
    case Op::BitwiseNot:
        return CanEvaluateTerm(exp.src1, env);
    default:
        return CanEvaluateTerm(exp.src1, env) && CanEvaluateTerm(exp.src2, env);
    }
}

auto EvaluateTerm(const string &term, const map<string, int> &env) -> int {
    if (IsNumber(term)) {
        return stoi(term);
    }

    return env.at(term);
}

auto Evaluate(const Expression &exp, map<string, int> &env) -> bool {
    if (!CanEvaluate(exp, env)) {
        return false;
    }

    switch (exp.op) {
    case Op::Assignment:
        env.insert({exp.dest, EvaluateTerm(exp.src1, env)});
        break;
    case Op::BitwiseNot:
        env.insert({exp.dest, ~EvaluateTerm(exp.src1, env) & 0xffff});
        break;
    case Op::BitwiseAnd: {
        int val1 = EvaluateTerm(exp.src1, env);
        int val2 = EvaluateTerm(exp.src2, env);
        env.insert({exp.dest, val1 & val2});
        break;
    }
    case Op::BitwiseOr: {
        int val1 = EvaluateTerm(exp.src1, env);
        int val2 = EvaluateTerm(exp.src2, env);
        env.insert({exp.dest, val1 | val2});
        break;
    }
    case Op::LeftShift: {
        int val1 = EvaluateTerm(exp.src1, env);
        int val2 = EvaluateTerm(exp.src2, env);
        env.insert({exp.dest, val1 << val2});
        break;
    }
    case Op::RightShift: {
        int val1 = EvaluateTerm(exp.src1, env);
        int val2 = EvaluateTerm(exp.src2, env);
        env.insert({exp.dest, val1 >> val2});
        break;
    }
    }
    return true;
}

auto Problem1(const vector<Expression> &exps, const string &var_name) -> int {
    vector<bool> checked(exps.size(), false);
    map<string, int> env;
    while (true) {
        bool evaluated = false;
        for (size_t i = 0; i < exps.size(); ++i) {
            if (checked[i]) {
                continue;
            }

            if (Evaluate(exps[i], env)) {
                evaluated = true;
                checked[i] = true;
            }
        }

        if (!evaluated) {
            break;
        }
    }

    assert(env.contains(var_name));
    return env.at(var_name);
}

auto Test() {
    const vector<string> input{"123 -> x",        "456 -> y",        "x AND y -> d", "x OR y -> e",
                               "x LSHIFT 2 -> f", "y RSHIFT 2 -> g", "NOT x -> h",   "NOT y -> i"};
    vector<Expression> exps;
    for (const auto &s : input) {
        exps.push_back(Parse(s));
    }

    const vector<pair<string, int>> test_data{
        {"d", 72}, {"e", 507}, {"f", 492}, {"g", 114}, {"h", 65412}, {"i", 65079}, {"x", 123}, {"y", 456},
    };

    for (const auto &d : test_data) {
        int ret = Problem1(exps, d.first);
        assert(ret == d.second);
    }
}

int main() {
    Test();

    ifstream f("../input/day07.txt");
    vector<Expression> input;
    string tmp;

    while (getline(f, tmp)) {
        input.push_back(Parse(tmp));
    }

    int ret1 = Problem1(input, "a");
    assert(ret1 == 16076);

    printf("ret1 = %d\n", ret1);

    return 0;
}
