#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <regex>
#include <functional>

namespace {

struct Rule {
    std::string name;
    std::vector<std::vector<std::int64_t>> ranges;

    bool operator==(const Rule &other) const {
        return name == other.name && ranges == other.ranges;
    }

    bool Valid(std::int64_t number) const {
        for (const auto &range : ranges) {
            if (number >= range[0] && number <= range[1]) {
                return true;
            }
        }

        return false;
    }
};

struct TicketData {
    std::vector<Rule> rules;
    std::vector<std::int64_t> my_ticket;
    std::vector<std::vector<std::int64_t>> near_by_tickets;

    std::int64_t Solve01() {
        std::int64_t ret = 0;
        for (const auto &near_by : near_by_tickets) {
            for (auto number : near_by) {
                bool valid = false;
                for (const auto &rule : rules) {
                    if (rule.Valid(number)) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    ret += number;
                }
            }
        }

        return ret;
    }

    std::vector<std::vector<std::int64_t>> CollectValidTickets() const {
        std::vector<std::vector<std::int64_t>> v;

        for (const auto &near_by : near_by_tickets) {
            bool all_ok = true;

            for (auto number : near_by) {
                bool valid = false;
                for (const auto &rule : rules) {
                    if (rule.Valid(number)) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    all_ok = false;
                    break;
                }
            }

            if (all_ok) {
                v.push_back(near_by);
            }
        }

        return v;
    }

    std::map<std::size_t, size_t> DicideNames(const std::vector<std::vector<std::int64_t>> &tickets) {
        size_t limit = tickets[0].size();
        std::function<std::map<size_t, size_t>(size_t pos, const std::map<size_t, size_t> &acc)> f;
        f = [&f, &tickets, this, limit](size_t pos, const std::map<size_t, size_t> &acc) -> std::map<size_t, size_t> {
            if (pos == limit) {
                return acc;
            }

            std::map<std::size_t, std::size_t> ret;
            for (size_t i = 0; i < rules.size(); ++i) {
                if (acc.find(i) != acc.end()) {
                    continue;
                }

                bool valid = true;
                for (const auto &ticket : tickets) {
                    if (!rules[i].Valid(ticket[pos])) {
                        valid = false;
                        break;
                    }
                }

                if (valid) {
                    auto tmp = acc;
                    tmp[i] = pos;
                    ret = f(pos + 1, tmp);
                    if (!ret.empty()) {
                        return ret;
                    }
                }
            }

            return ret;
        };

        return f(0, std::map<size_t, size_t>{});
    }

    std::int64_t Solve02(const std::string &rule_prefix) {
        auto valid_tickets = CollectValidTickets();
        auto indices = DicideNames(valid_tickets);

        std::int64_t ret = 1;
        for (size_t i = 0; i < rules.size(); ++i) {
            if (rules[i].name.find(rule_prefix) != 0) {
                continue;
            }

            ret *= my_ticket[indices[i]];
        }

        return ret;
    }
};

TicketData ParseInput(const std::string &input) {
    std::istringstream ss(input);
    std::string line;

    enum class State {
        kRule,
        kYourTicket,
        kNearByTicket,
    };

    std::regex range_re("(\\d+)-(\\d+)");

    TicketData ret;
    State state = State::kRule;
    while (std::getline(ss, line)) {
        switch (state) {
        case State::kRule: {
            if (line.empty()) {
                state = State::kYourTicket;
                break;
            }

            Rule rule;
            size_t pos = line.find(':');
            rule.name = line.substr(0, pos);
            std::smatch match;

            while (std::regex_search(line, match, range_re)) {
                std::int64_t min = std::stoi(match[1]);
                std::int64_t max = std::stoi(match[2]);

                rule.ranges.push_back(std::vector<std::int64_t>{min, max});
                line = match.suffix();
            }

            ret.rules.push_back(rule);
            break;
        }
        case State::kYourTicket: {
            if (line.empty()) {
                state = State::kNearByTicket;
                break;
            }

            if (!(line[0] >= '0' && line[0] <= '9')) {
                break;
            }

            std::istringstream lss(line);
            std::string number;
            while (std::getline(lss, number, ',')) {
                std::int64_t num = std::stoi(number);
                ret.my_ticket.push_back(num);
            }

            break;
        }
        case State::kNearByTicket: {
            if (!(line[0] >= '0' && line[0] <= '9')) {
                break;
            }

            std::istringstream lss(line);
            std::string number;
            std::vector<std::int64_t> numbers;
            while (std::getline(lss, number, ',')) {
                std::int64_t num = std::stoi(number);
                numbers.push_back(num);
            }

            ret.near_by_tickets.emplace_back(std::move(numbers));
            break;
        }
        }
    }

    return ret;
}

void Test01() {
    std::string input(R"(class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12)");

    auto ticket = ParseInput(input);
    assert(ticket.rules.size() == 3);
    assert(ticket.rules[0].name == "class");
    assert(ticket.rules[0].ranges[0][0] == 1);
    assert(ticket.rules[0].ranges[0][1] == 3);
    assert(ticket.rules[0].ranges[1][0] == 5);
    assert(ticket.rules[0].ranges[1][1] == 7);
    assert(ticket.rules[1].name == "row");
    assert(ticket.rules[1].ranges[0][0] == 6);
    assert(ticket.rules[1].ranges[0][1] == 11);
    assert(ticket.rules[1].ranges[1][0] == 33);
    assert(ticket.rules[1].ranges[1][1] == 44);
    assert(ticket.rules[2].name == "seat");
    assert(ticket.rules[2].ranges[0][0] == 13);
    assert(ticket.rules[2].ranges[0][1] == 40);
    assert(ticket.rules[2].ranges[1][0] == 45);
    assert(ticket.rules[2].ranges[1][1] == 50);

    assert((ticket.my_ticket == std::vector<std::int64_t>{7, 1, 14}));

    assert(ticket.near_by_tickets.size() == 4);
    assert((ticket.near_by_tickets[0] == std::vector<std::int64_t>{7, 3, 47}));
    assert((ticket.near_by_tickets[1] == std::vector<std::int64_t>{40, 4, 50}));
    assert((ticket.near_by_tickets[2] == std::vector<std::int64_t>{55, 2, 20}));
    assert((ticket.near_by_tickets[3] == std::vector<std::int64_t>{38, 6, 12}));

    assert(ticket.Solve01() == 71);
}

void Test02() {
    std::string input(R"(departure one: 0-1 or 4-19
departure two: 0-5 or 8-19
seat: 0-13 or 16-19

your ticket:
11,12,13

nearby tickets:
3,9,18
15,1,5
5,14,9)");

    auto ticket = ParseInput(input);
    assert(ticket.CollectValidTickets().size() == 3);

    assert(ticket.Solve02("departure") == 132);
}

} // namespace

int main() {
    Test01();
    Test02();

    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input += line + "\n";
    }

    auto ticket = ParseInput(input);
    std::cout << "Part01: " << ticket.Solve01() << std::endl;
    std::cout << "Part02: " << ticket.Solve02("departure") << std::endl;
    return 0;
}
