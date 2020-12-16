#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <regex>

namespace {

struct Rule {
    int min;
    int max;

    bool operator==(const Rule &other) const {
        return min == other.min && max == other.max;
    }

    bool Valid(int number) const {
        return number >= min && number <= max;
    }
};

struct Ticket {
    std::vector<int> numbers;
};

struct TicketData {
    std::map<std::string, std::vector<Rule>> rules;
    Ticket my_ticket;
    std::vector<Ticket> near_by_tickets;

    int Solve01() {
        int ret = 0;
        for (const auto &near_by : near_by_tickets) {
            for (auto number : near_by.numbers) {
                bool valid = false;
                for (const auto &it : rules) {
                    for (const auto &rule : it.second) {
                        if (rule.Valid(number)) {
                            valid = true;
                            break;
                        }
                    }

                    if (valid) {
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

    int Solve02() {
        int ret = 0;
        for (const auto &near_by : near_by_tickets) {
            for (auto number : near_by.numbers) {
                bool valid = false;
                for (const auto &it : rules) {
                    for (const auto &rule : it.second) {
                        if (rule.Valid(number)) {
                            valid = true;
                            break;
                        }
                    }

                    if (valid) {
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

            size_t pos = line.find(':');
            std::string name = line.substr(0, pos);
            std::smatch match;
            while (std::regex_search(line, match, range_re)) {
                int min = std::stoi(match[1]);
                int max = std::stoi(match[2]);

                ret.rules[name].emplace_back(Rule{min, max});
                line = match.suffix();
            }
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
    assert((ticket.rules["class"][0] == Rule{1, 3}));
    assert((ticket.rules["class"][1] == Rule{5, 7}));
    assert((ticket.rules["row"][0] == Rule{6, 11}));
    assert((ticket.rules["row"][1] == Rule{33, 44}));
    assert((ticket.rules["seat"][0] == Rule{13, 40}));
    assert((ticket.rules["seat"][1] == Rule{45, 50}));

    assert((ticket.my_ticket.numbers == std::vector<int>{7, 1, 14}));

    assert(ticket.near_by_tickets.size() == 4);
    assert((ticket.near_by_tickets[0].numbers == std::vector<int>{7, 3, 47}));
    assert((ticket.near_by_tickets[1].numbers == std::vector<int>{40, 4, 50}));
    assert((ticket.near_by_tickets[2].numbers == std::vector<int>{55, 2, 20}));
    assert((ticket.near_by_tickets[3].numbers == std::vector<int>{38, 6, 12}));

    assert(ticket.Solve01() == 71);
}

} // namespace

int main() {
    Test01();

    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input += line + "\n";
    }

    auto ticket = ParseInput(input);
    std::cout << "Part01: " << ticket.Solve01() << std::endl;
    return 0;
}
