#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>

namespace {

struct Food {
    std::set<std::string> ingredients;
    std::set<std::string> allergens;

    bool operator==(const Food &other) const {
        return ingredients == other.ingredients && allergens == other.allergens;
    }
};

template <typename T>
std::vector<Food> ParseInput(T &input_stream) {
    std::vector<Food> foods;
    std::string line;
    while (std::getline(input_stream, line)) {
        Food food;
        size_t p = 0;
        bool is_ingredient_part = true;
        while (p < line.size()) {
            std::string str;
            while (p < line.size() && (line[p] != ' ' && line[p] != ')' && line[p] != ',')) {
                str.push_back(line[p]);
                ++p;
            }

            if (str == "(contains") {
                ++p;
                is_ingredient_part = false;
                continue;
            }

            if (is_ingredient_part) {
                food.ingredients.insert(str);
            } else {
                food.allergens.insert(str);
            }

            if (line[p] == ')') {
                break;
            }

            while (p < line.size() && (line[p] == ' ' || line[p] == ')' || line[p] == ',')) {
                ++p;
            }
        }

        foods.push_back(food);
    }

    return foods;
}

int Solve01(const std::vector<Food> &foods) {
    std::set<std::string> all_ingredients;
    std::set<std::string> all_allergens;
    for (const auto &food : foods) {
        for (const auto &ingredient : food.ingredients) {
            all_ingredients.insert(ingredient);
        }
        for (const auto &allergen : food.allergens) {
            all_allergens.insert(allergen);
        }
    }

    std::map<std::string, std::set<std::string>> table;
    for (const auto &ingredient : all_ingredients) {
        table[ingredient] = all_allergens;
    }

    while (true) {
        for (auto &it : table) {
            auto &ingredient = it.first;

            for (const auto &food : foods) {
                if (food.ingredients.find(ingredient) == food.ingredients.end()) {
                    for (const auto &allergen : food.allergens) {
                        it.second.erase(allergen);
                    }
                }
            }

            if (it.second.size() == 1) {
                auto decided = (*it.second.begin());
                for (auto &it2 : table) {
                    if (it.first == it2.first) {
                        continue;
                    }
                    it2.second.erase(decided);
                }
            }
        }

        bool ok = true;
        for (const auto &it : table) {
            if (it.second.size() >= 2) {
                ok = false;
                break;
            }
        }

        if (ok) {
            break;
        }
    }

    std::set<std::string> safes;
    for (const auto &it : table) {
        if (it.second.empty()) {
            safes.insert(it.first);
        }
    }

    int ret = 0;
    for (const auto &safe : safes) {
        for (const auto &food : foods) {
            if (food.ingredients.find(safe) != food.ingredients.end()) {
                ++ret;
            }
        }
    }

    return ret;
}

void Test01() {
    std::string input(R"(mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
trh fvjkl sbzzf mxmxvkd (contains dairy)
sqjhc fvjkl (contains soy)
sqjhc mxmxvkd sbzzf (contains fish))");

    std::istringstream ss(input);
    auto foods = ParseInput(ss);
    assert(foods.size() == 4);
    {
        std::set<std::string> ingredients{"mxmxvkd", "kfcds", "sqjhc", "nhms"};
        std::set<std::string> allergens{"dairy", "fish"};
        assert((foods[0] == Food{ingredients, allergens}));
    }
    {
        std::set<std::string> ingredients{"trh", "fvjkl", "sbzzf", "mxmxvkd"};
        std::set<std::string> allergens{"dairy"};
        assert((foods[1] == Food{ingredients, allergens}));
    }
    {
        std::set<std::string> ingredients{"sqjhc", "fvjkl"};
        std::set<std::string> allergens{"soy"};
        assert((foods[2] == Food{ingredients, allergens}));
    }
    {
        std::set<std::string> ingredients{"sqjhc", "mxmxvkd", "sbzzf"};
        std::set<std::string> allergens{"fish"};
        assert((foods[3] == Food{ingredients, allergens}));
    }

    assert(Solve01(foods) == 5);
}

} // namespace

int main() {
    Test01();

    auto foods = ParseInput(std::cin);
    std::cout << "Part01:" << Solve01(foods) << std::endl;
    return 0;
}
