#include <cassert>
#include <iostream>
#include <sstream>
#include <set>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <tuple>
#include <optional>

namespace {

struct Data {
    int x1;
    int x2;
    int y1;
    int y2;
    int z1;
    int z2;
    bool on;

    bool operator<(const Data &o) const {
        return std::tie(x1, x2, y1, y2, z1, z2) < std::tie(o.x1, o.x2, o.y1, o.y2, o.z1, o.z2);
    }

    bool operator==(const Data &o) const {
        return std::tie(x1, x2, y1, y2, z1, z2) == std::tie(o.x1, o.x2, o.y1, o.y2, o.z1, o.z2);
    }

    void Print() const {
        printf("x=%d..%d,y=%d..%d,z=%d..%d\n", x1, x2, y1, y2, z1, z2);
    }

    std::string ToString() const {
        std::stringstream ss;
        ss << "x=" << x1 << ".." << x2 << ",";
        ss << "y=" << y1 << ".." << y2 << ",";
        ss << "z=" << z1 << ".." << z2;
        return ss.str();
    }

    std::int64_t Volume() const noexcept {
        std::int64_t x = x2 - x1 + 1;
        std::int64_t y = y2 - y1 + 1;
        std::int64_t z = z2 - z1 + 1;
        return x * y * z;
    }

    bool IsCrossing(const Data &o) const noexcept {
        const auto f = [](int a_min, int a_max, int b_min, int b_max) -> bool {
            return std::min(a_max, b_max) - std::max(a_min, b_min) >= 0;
        };

        return f(x1, x2, o.x1, o.x2) && f(y1, y2, o.y1, o.y2) && f(z1, z2, o.z1, o.z2);
    }

    bool Contain(const Data &o) const noexcept {
        if (!(x1 <= o.x1 && o.x1 <= x2 && x1 <= o.x2 && o.x2 <= x2)) {
            return false;
        }
        if (!(y1 <= o.y1 && o.y1 <= y2 && y1 <= o.y2 && o.y2 <= y2)) {
            return false;
        }
        if (!(z1 <= o.z1 && o.z1 <= z2 && z1 <= o.z2 && o.z2 <= z2)) {
            return false;
        }
        return true;
    }

    std::optional<Data> CrossRegion(const Data &o) const noexcept {
        if (!IsCrossing(o)) {
            return std::nullopt;
        }

        int max_x1 = std::max(x1, o.x1);
        int min_x2 = std::min(x2, o.x2);
        int max_y1 = std::max(y1, o.y1);
        int min_y2 = std::min(y2, o.y2);
        int max_z1 = std::max(z1, o.z1);
        int min_z2 = std::min(z2, o.z2);
        return std::make_optional<Data>(Data{max_x1, min_x2, max_y1, min_y2, max_z1, min_z2});
    }

    static std::vector<std::vector<int>> Split(int a_min, int a_max, int b_min, int b_max) {
        int small_min = std::min(a_min, b_min);
        int large_min = std::max(a_min, b_min);
        int small_max = std::min(a_max, b_max);
        int large_max = std::max(a_max, b_max);

        std::vector<std::vector<int>> ret;
        if (small_min <= large_min) {
            ret.push_back({small_min, large_min - 1});
        }
        if (large_min <= small_max) {
            ret.push_back({large_min, small_max});
        }
        if (small_max <= large_max) {
            ret.push_back({small_max + 1, large_max});
        }
        return ret;
    }

    std::set<Data> SplitCubes(const Data &o) const {
        auto v1 = Volume();
        auto v2 = o.Volume();
        auto cross = CrossRegion(o);
        if (!cross.has_value()) {
            return {*this, o};
        }

        auto total = v1 + v2 - cross.value().Volume();

        std::set<Data> ret;
        auto x_regions = Split(x1, x2, o.x1, o.x2);
        auto y_regions = Split(y1, y2, o.y1, o.y2);
        auto z_regions = Split(z1, z2, o.z1, o.z2);

        std::int64_t n = 0;
        for (const auto &x : x_regions) {
            for (const auto &y : y_regions) {
                for (const auto &z : z_regions) {
                    Data new_region{x[0], x[1], y[0], y[1], z[0], z[1], false};
                    if (!(Contain(new_region) || o.Contain(new_region))) {
                        continue;
                    }
                    ret.insert(new_region);
                    n += new_region.Volume();
                }
            }
        }

        return ret;
    }
};

std::vector<Data> ParseInput(std::istream &ss) {
    std::vector<Data> ret;

    std::string tmp;
    int x1, x2, y1, y2, z1, z2;
    while (std::getline(ss, tmp)) {
        if (tmp.empty()) {
            continue;
        }

        bool on;
        if (strncmp(tmp.c_str(), "on", 2) == 0) {
            on = true;
#if _WIN32
            sscanf_s(tmp.c_str() + 3, "x=%d..%d,y=%d..%d,z=%d..%d", &x1, &x2, &y1, &y2, &z1, &z2);
#else
            sscanf(tmp.c_str() + 3, "x=%d..%d,y=%d..%d,z=%d..%d", &x1, &x2, &y1, &y2, &z1, &z2);
#endif
        } else {
            on = false;
#if _WIN32
            sscanf_s(tmp.c_str() + 4, "x=%d..%d,y=%d..%d,z=%d..%d", &x1, &x2, &y1, &y2, &z1, &z2);
#else
            sscanf(tmp.c_str() + 4, "x=%d..%d,y=%d..%d,z=%d..%d", &x1, &x2, &y1, &y2, &z1, &z2);
#endif
        }

        ret.push_back({x1, x2, y1, y2, z1, z2, on});
    }

    return ret;
}

size_t Part1(const std::vector<Data> &data) {
    std::set<std::tuple<int, int, int>> ret;
    for (const auto &d : data) {
        int min_x = std::max(d.x1, -50);
        int max_x = std::min(d.x2, 50);
        int min_y = std::max(d.y1, -50);
        int max_y = std::min(d.y2, 50);
        int min_z = std::max(d.z1, -50);
        int max_z = std::min(d.z2, 50);

        for (int i = min_x; i <= max_x; ++i) {
            for (int j = min_y; j <= max_y; ++j) {
                for (int k = min_z; k <= max_z; ++k) {
                    const std::tuple<int, int, int> key{i, j, k};
                    if (d.on) {
                        ret.insert(key);
                    } else {
                        const auto &it = ret.find(key);
                        if (it != ret.end()) {
                            ret.erase(it);
                        }
                    }
                }
            }
        }
    }

    return ret.size();
}

size_t Part2(const std::vector<Data> &data, int region_size) {
    std::set<Data> cuboids;

    for (const auto &d : data) {
        bool has_crossing = false;

        std::set<Data> remaining_regions{d};
        std::set<Data> tmp = cuboids;
        for (const auto &cuboid : cuboids) {
            const auto &crossed = cuboid.CrossRegion(d);
            if (!crossed.has_value()) {
                continue;
            }

            has_crossing = true;
            tmp.erase(cuboid);

            if (d.on) {
                std::set<Data> new_regions;
                for (const auto &k : remaining_regions) {
                    auto cubes = k.SplitCubes(cuboid);
                    for (const auto &cube : cubes) {
                        new_regions.insert(cube);
                    }
                }
                remaining_regions = new_regions;
            } else {
                auto cubes = d.SplitCubes(cuboid);
                for (auto it = cubes.begin(); it != cubes.end();) {
                    if (d.Contain(*it)) {
                        it = cubes.erase(it);
                    } else {
                        tmp.insert(*it);
                        ++it;
                    }
                }
            }
        }

        if (d.on) {
            if (has_crossing) {
                cuboids = tmp;
                for (const auto &r : remaining_regions) {
                    cuboids.insert(r);
                }

                std::set<Data> not_included_regions;
                for (const auto &a : cuboids) {
                    bool ok = true;
                    for (const auto &b : cuboids) {
                        if (a == b) {
                            continue;
                        }
                        if (b.Contain(a)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        not_included_regions.insert(a);
                    }
                }
                cuboids = not_included_regions;
            } else {
                cuboids.insert(d);
            }
        } else {
            cuboids = tmp;
        }
    }

    std::int64_t ret = 0;
    Data init{-region_size, region_size, -region_size, region_size, -region_size, region_size};
    for (const auto &cuboid : cuboids) {
        auto r = init.CrossRegion(cuboid);
        if (r.has_value()) {
            ret += r.value().Volume();
        }
    }

    return ret;
}

void Test() {
    {
        std::string input(R"(on x=10..12,y=10..12,z=10..12
on x=11..13,y=11..13,z=11..13
off x=9..11,y=9..11,z=9..11
on x=10..10,y=10..10,z=10..10)");
        std::stringstream ss(input);
        auto data = ParseInput(ss);
        assert(Part1(data) == 39);
        assert(Part2(data, 50) == 39);
    }
    {
        std::string input(R"(on x=0..9,y=0..9,z=0..19
on x=20..29,y=0..9,z=0..19
on x=0..29,y=0..9,z=10..19)");
        std::stringstream ss(input);
        auto data = ParseInput(ss);
        assert(Part2(data, 50) == 5000);
    }
    {
        std::string input(R"(on x=10..12,y=10..12,z=10..12
off x=11..12,y=11..12,z=11..12)");
        std::stringstream ss(input);
        auto data = ParseInput(ss);
        assert(Part2(data, 50) == 19);
    }
    {
        std::string input(R"(on x=-20..26,y=-36..17,z=-47..7
on x=-20..33,y=-21..23,z=-26..28
on x=-22..28,y=-29..23,z=-38..16
on x=-46..7,y=-6..46,z=-50..-1
on x=-49..1,y=-3..46,z=-24..28
on x=2..47,y=-22..22,z=-23..27
on x=-27..23,y=-28..26,z=-21..29
on x=-39..5,y=-6..47,z=-3..44
on x=-30..21,y=-8..43,z=-13..34
on x=-22..26,y=-27..20,z=-29..19
off x=-48..-32,y=26..41,z=-47..-37
on x=-12..35,y=6..50,z=-50..-2
off x=-48..-32,y=-32..-16,z=-15..-5
on x=-18..26,y=-33..15,z=-7..46
off x=-40..-22,y=-38..-28,z=23..41
on x=-16..35,y=-41..10,z=-47..6
off x=-32..-23,y=11..30,z=-14..3
on x=-49..-5,y=-3..45,z=-29..18
off x=18..30,y=-20..-8,z=-3..13
on x=-41..9,y=-7..43,z=-33..15
on x=-54112..-39298,y=-85059..-49293,z=-27449..7877
on x=967..23432,y=45373..81175,z=27513..53682)");
        std::stringstream ss(input);
        auto data = ParseInput(ss);
        assert(Part1(data) == 590784);
        assert(Part2(data, 50) == 590784);
    }
    {
        std::string input(R"(on x=-5..47,y=-31..22,z=-19..33
on x=-44..5,y=-27..21,z=-14..35
on x=-49..-1,y=-11..42,z=-10..38
on x=-20..34,y=-40..6,z=-44..1
off x=26..39,y=40..50,z=-2..11
on x=-41..5,y=-41..6,z=-36..8
off x=-43..-33,y=-45..-28,z=7..25
on x=-33..15,y=-32..19,z=-34..11
off x=35..47,y=-46..-34,z=-11..5
on x=-14..36,y=-6..44,z=-16..29
on x=-57795..-6158,y=29564..72030,z=20435..90618
on x=36731..105352,y=-21140..28532,z=16094..90401
on x=30999..107136,y=-53464..15513,z=8553..71215
on x=13528..83982,y=-99403..-27377,z=-24141..23996
on x=-72682..-12347,y=18159..111354,z=7391..80950
on x=-1060..80757,y=-65301..-20884,z=-103788..-16709
on x=-83015..-9461,y=-72160..-8347,z=-81239..-26856
on x=-52752..22273,y=-49450..9096,z=54442..119054
on x=-29982..40483,y=-108474..-28371,z=-24328..38471
on x=-4958..62750,y=40422..118853,z=-7672..65583
on x=55694..108686,y=-43367..46958,z=-26781..48729
on x=-98497..-18186,y=-63569..3412,z=1232..88485
on x=-726..56291,y=-62629..13224,z=18033..85226
on x=-110886..-34664,y=-81338..-8658,z=8914..63723
on x=-55829..24974,y=-16897..54165,z=-121762..-28058
on x=-65152..-11147,y=22489..91432,z=-58782..1780
on x=-120100..-32970,y=-46592..27473,z=-11695..61039
on x=-18631..37533,y=-124565..-50804,z=-35667..28308
on x=-57817..18248,y=49321..117703,z=5745..55881
on x=14781..98692,y=-1341..70827,z=15753..70151
on x=-34419..55919,y=-19626..40991,z=39015..114138
on x=-60785..11593,y=-56135..2999,z=-95368..-26915
on x=-32178..58085,y=17647..101866,z=-91405..-8878
on x=-53655..12091,y=50097..105568,z=-75335..-4862
on x=-111166..-40997,y=-71714..2688,z=5609..50954
on x=-16602..70118,y=-98693..-44401,z=5197..76897
on x=16383..101554,y=4615..83635,z=-44907..18747
off x=-95822..-15171,y=-19987..48940,z=10804..104439
on x=-89813..-14614,y=16069..88491,z=-3297..45228
on x=41075..99376,y=-20427..49978,z=-52012..13762
on x=-21330..50085,y=-17944..62733,z=-112280..-30197
on x=-16478..35915,y=36008..118594,z=-7885..47086
off x=-98156..-27851,y=-49952..43171,z=-99005..-8456
off x=2032..69770,y=-71013..4824,z=7471..94418
on x=43670..120875,y=-42068..12382,z=-24787..38892
off x=37514..111226,y=-45862..25743,z=-16714..54663
off x=25699..97951,y=-30668..59918,z=-15349..69697
off x=-44271..17935,y=-9516..60759,z=49131..112598
on x=-61695..-5813,y=40978..94975,z=8655..80240
off x=-101086..-9439,y=-7088..67543,z=33935..83858
off x=18020..114017,y=-48931..32606,z=21474..89843
off x=-77139..10506,y=-89994..-18797,z=-80..59318
off x=8476..79288,y=-75520..11602,z=-96624..-24783
on x=-47488..-1262,y=24338..100707,z=16292..72967
off x=-84341..13987,y=2429..92914,z=-90671..-1318
off x=-37810..49457,y=-71013..-7894,z=-105357..-13188
off x=-27365..46395,y=31009..98017,z=15428..76570
off x=-70369..-16548,y=22648..78696,z=-1892..86821
on x=-53470..21291,y=-120233..-33476,z=-44150..38147
off x=-93533..-4276,y=-16170..68771,z=-104985..-24507)");

        std::stringstream ss(input);
        auto data = ParseInput(ss);
        auto part2 = Part2(data, 474140);
        assert(part2 == 2758514936282235);
    }
}

} // namespace

int main() {
    Test();

    auto data = ParseInput(std::cin);
    auto part1 = Part1(data);
    auto part2 = Part2(data, 474140);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}