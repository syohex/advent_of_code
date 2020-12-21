#include <cassert>
#include <vector>
#include <string>
#include <sstream>

namespace {

struct Food {
    std::vector<std::string> ingredients;
    std::vector<std::string> allergens;

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
                food.ingredients.push_back(str);
            } else {
                food.allergens.push_back(str);
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

void Test01() {
    std::string input(R"(mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
trh fvjkl sbzzf mxmxvkd (contains dairy)
sqjhc fvjkl (contains soy)
sqjhc mxmxvkd sbzzf (contains fish))");

    std::istringstream ss(input);
    auto foods = ParseInput(ss);
    assert(foods.size() == 4);
    {
        std::vector<std::string> ingredients{"mxmxvkd", "kfcds", "sqjhc", "nhms"};
        std::vector<std::string> allergens{"dairy", "fish"};
        assert((foods[0] == Food{ingredients, allergens}));
    }
    {
        std::vector<std::string> ingredients{"trh", "fvjkl", "sbzzf", "mxmxvkd"};
        std::vector<std::string> allergens{"dairy"};
        assert((foods[1] == Food{ingredients, allergens}));
    }
    {
        std::vector<std::string> ingredients{"sqjhc", "fvjkl"};
        std::vector<std::string> allergens{"soy"};
        assert((foods[2] == Food{ingredients, allergens}));
    }
    {
        std::vector<std::string> ingredients{"sqjhc", "mxmxvkd", "sbzzf"};
        std::vector<std::string> allergens{"fish"};
        assert((foods[3] == Food{ingredients, allergens}));
    }
}

} // namespace

int main() {
    Test01();
    return 0;
}
