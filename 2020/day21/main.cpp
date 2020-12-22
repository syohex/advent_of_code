#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>

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

std::map<std::string, std::set<std::string>> DetermineAllergen(const std::vector<Food> &foods) {
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

    std::map<std::string, std::set<std::string>> ingredient_table;
    for (const auto &ingredient : all_ingredients) {
        ingredient_table[ingredient] = all_allergens;
    }

    while (true) {
        for (auto &it : ingredient_table) {
            auto &ingredient = it.first;
            auto &allergens = it.second;

            for (const auto &food : foods) {
                if (food.ingredients.find(ingredient) == food.ingredients.end()) {
                    for (const auto &allergen : food.allergens) {
                        allergens.erase(allergen);
                    }
                }
            }

            if (allergens.size() == 1) {
                auto allergen = (*allergens.begin());
                for (auto &it2 : ingredient_table) {
                    if (ingredient == it2.first) { // ignore me
                        continue;
                    }

                    it2.second.erase(allergen);
                }
            }
        }

        bool all_determined = true;
        for (const auto &it : ingredient_table) {
            const auto &allergens = it.second;
            if (allergens.size() >= 2) {
                all_determined = false;
                break;
            }
        }

        if (all_determined) {
            break;
        }
    }

    return ingredient_table;
}

int Solve01(const std::vector<Food> &foods) {
    auto ingredient_table = DetermineAllergen(foods);

    std::set<std::string> safe_ingredients;
    for (const auto &it : ingredient_table) {
        const auto &ingredient = it.first;
        const auto &allergens = it.second;
        if (allergens.empty()) {
            safe_ingredients.insert(ingredient);
        }
    }

    int ret = 0;
    for (const auto &ingredient : safe_ingredients) {
        for (const auto &food : foods) {
            if (food.ingredients.find(ingredient) != food.ingredients.end()) {
                ++ret;
            }
        }
    }

    return ret;
}

std::string Solve02(const std::vector<Food> &foods) {
    auto ingredient_table = DetermineAllergen(foods);

    struct Data {
        std::string ingredient;
        std::string allergen;
    };

    std::vector<Data> dangerous_ingredients;
    for (const auto &it : ingredient_table) {
        const auto &ingredient = it.first;
        const auto &allergens = it.second;
        if (!allergens.empty()) {
            assert(allergens.size() == 1);
            const auto &allergen = *(allergens.begin());
            dangerous_ingredients.emplace_back(Data{ingredient, allergen});
        }
    }

    std::sort(dangerous_ingredients.begin(), dangerous_ingredients.end(),
              [](const Data &a, const Data &b) { return a.allergen < b.allergen; });

    std::string ret;
    size_t limit = dangerous_ingredients.size();
    for (size_t i = 0; i < limit; ++i) {
        ret += dangerous_ingredients[i].ingredient;
        if (i != limit - 1) {
            ret += ",";
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
    assert(Solve02(foods) == "mxmxvkd,sqjhc,fvjkl");
}

} // namespace

int main() {
    Test01();

    auto foods = ParseInput(std::cin);
    std::cout << "Part01: " << Solve01(foods) << std::endl;
    std::cout << "Part02: " << Solve02(foods) << std::endl;
    return 0;
}
