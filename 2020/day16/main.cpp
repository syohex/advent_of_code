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
    std::vector<std::vector<int>> ranges;

    bool operator==(const Rule &other) const {
        return name == other.name && ranges == other.ranges;
    }

    bool Valid(int number) const {
        for (const auto &range : ranges) {
            if (number >= range[0] && number <= range[1]) {
                return true;
            }
        }

        return false;
    }
};

struct Ticket {
    std::vector<int> numbers;
};

struct TicketData {
    std::vector<Rule> rules;
    Ticket my_ticket;
    std::vector<Ticket> near_by_tickets;

    int Solve01() {
        int ret = 0;
        for (const auto &near_by : near_by_tickets) {
            for (auto number : near_by.numbers) {
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

    std::vector<Ticket> CollectValidTickets() const {
        std::vector<Ticket> v;

        for (const auto &near_by : near_by_tickets) {
            bool all_ok = true;

            for (auto number : near_by.numbers) {
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

    std::map<std::size_t, size_t> DicideNames(const std::vector<Ticket> &tickets) {
        std::vector<std::string> rule_names;
        for (const auto &rule : rules) {
            rule_names.push_back(rule.name);
        }

        std::map<size_t, size_t> ret;
        size_t limit = tickets[0].numbers.size();
        std::function<void(size_t pos, const std::map<size_t, size_t> &acc)> f;
        f = [&f, &tickets, this, limit, &ret](size_t pos, const std::map<size_t, size_t> &acc) {
            if (pos == limit) {
                ret = acc;
                return;
            }

            for (size_t i = 0; i < rules.size(); ++i) {
                if (acc.find(i) != acc.end()) {
                    continue;
                }

                bool valid = true;
                for (const auto &ticket : tickets) {
                    if (!rules[i].Valid(ticket.numbers[pos])) {
                        valid = false;
                        break;
                    }
                }

                if (!valid) {
                    auto tmp = acc;
                    tmp[i] = pos;
                    f(pos + 1, tmp);
                }
            }
        };

        f(0, std::map<size_t, size_t>{});

        for (const auto &it : ret) {
            printf("@@ %s -> %zd\n", rules[it.first].name.c_str(), it.second);
        }

        return ret;
    }

    int Solve02() {
        auto valid_tickets = CollectValidTickets();
        auto indices = DicideNames(valid_tickets);
        return 0;
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
                int min = std::stoi(match[1]);
                int max = std::stoi(match[2]);

                rule.ranges.push_back(std::vector<int>{min, max});
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
                int num = std::stoi(number);
                ret.my_ticket.numbers.push_back(num);
            }

            break;
        }
        case State::kNearByTicket: {
            if (!(line[0] >= '0' && line[0] <= '9')) {
                break;
            }

            std::istringstream lss(line);
            std::string number;
            std::vector<int> numbers;
            while (std::getline(lss, number, ',')) {
                int num = std::stoi(number);
                numbers.push_back(num);
            }

            ret.near_by_tickets.emplace_back(Ticket{numbers});
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

    assert((ticket.my_ticket.numbers == std::vector<int>{7, 1, 14}));

    assert(ticket.near_by_tickets.size() == 4);
    assert((ticket.near_by_tickets[0].numbers == std::vector<int>{7, 3, 47}));
    assert((ticket.near_by_tickets[1].numbers == std::vector<int>{40, 4, 50}));
    assert((ticket.near_by_tickets[2].numbers == std::vector<int>{55, 2, 20}));
    assert((ticket.near_by_tickets[3].numbers == std::vector<int>{38, 6, 12}));

    assert(ticket.Solve01() == 71);
}

void Test02() {
    std::string input(R"(class: 0-1 or 4-19
row: 0-5 or 8-19
seat: 0-13 or 16-19

your ticket:
11,12,13

nearby tickets:
3,9,18
15,1,5
5,14,9)");

    auto ticket = ParseInput(input);
    assert(ticket.CollectValidTickets().size() == 3);

    ticket.Solve02();
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
    std::cout << "Part02: " << ticket.Solve02() << std::endl;
    return 0;
}
