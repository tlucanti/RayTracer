
#ifndef RTX_COLOR_HPP
# define RTX_COLOR_HPP

# include <common.hpp>

RTX_NAMESPACE_BEGIN

struct Color
{
    const char *_col;

    std::string operator [](const std::string &str) const
    {
        return _col + str + reset;
    }

    static constexpr char black[] = "\033[1;90m";
    static constexpr char red[] = "\033[1;91m";
    static constexpr char green[] = "\033[1;92m";
    static constexpr char yellow[] = "\033[1;93m";
    static constexpr char blue[] = "\033[1;94m";
    static constexpr char purple[] = "\033[1;95m";
    static constexpr char cyan[] = "\033[1;96m";
    static constexpr char white[] = "\033[1;97m";
    static constexpr char reset[] = "\033[0m";

    static constexpr char dark_red[] = "\033[38;2;167;0;0;1m";

    static constexpr char orange[] = "\033[38;2;255;165;0;1m";
};

inline constexpr Color Black {Color::black};
inline constexpr Color Red {Color::red};
inline constexpr Color Green {Color::green};
inline constexpr Color Yellow {Color::yellow};
inline constexpr Color Blue {Color::blue};
inline constexpr Color Purple {Color::purple};
inline constexpr Color Cyan {Color::cyan};
inline constexpr Color White {Color::white};
inline constexpr Color Reset {Color::reset};

inline constexpr Color DarkRed {Color::dark_red};

inline constexpr const Color &Magenta = Purple;
inline constexpr const Color &K = Black;
inline constexpr const Color &R = Red;
inline constexpr const Color &G = Green;
inline constexpr const Color &Y = Yellow;
inline constexpr const Color &B = Blue;
inline constexpr const Color &P = Purple;
inline constexpr const Color &C = Cyan;
inline constexpr const Color &W = White;

inline constexpr const Color Orange {Color::orange};

RTX_NAMESPACE_END

inline std::string operator ""_K(const char *str, size_t) { return rtx::K[str]; }
inline std::string operator ""_R(const char *str, size_t) { return rtx::R[str]; }
inline std::string operator ""_G(const char *str, size_t) { return rtx::G[str]; }
inline std::string operator ""_Y(const char *str, size_t) { return rtx::Y[str]; }
inline std::string operator ""_B(const char *str, size_t) { return rtx::B[str]; }
inline std::string operator ""_P(const char *str, size_t) { return rtx::P[str]; }
inline std::string operator ""_C(const char *str, size_t) { return rtx::C[str]; }
inline std::string operator ""_W(const char *str, size_t) { return rtx::W[str]; }

#endif /* RTX_COLOR_HPP */
