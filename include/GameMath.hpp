#pragma once
#include <cmath>
#include <SFML/System.hpp>
#include <type_traits>
#include <cstdint>
#include <limits>
#include <Game.h>

// substitui std::common_type
// por http://stackoverflow.com/a/18285792/482238
template<class I, bool Signed>
struct mk_signed { typedef I type; };
template<>
struct mk_signed<uint8_t, true> { typedef int16_t type; };
template<>
struct mk_signed<uint16_t, true> { typedef int32_t type; };
template<>
struct mk_signed<uint32_t, true> { typedef int64_t type; };
template<>
struct mk_signed<uint64_t, true> { typedef int64_t type; };

template<typename... Ts>
struct best_common_numeric_type;
template<typename T>
struct best_common_numeric_type<T> { typedef T type; };

template<typename T, typename... Ts>
struct best_common_numeric_type<T, Ts...> {
    typedef typename best_common_numeric_type<Ts...>::type TS;
    typedef typename std::conditional<(sizeof(T) > sizeof(TS)), T, TS>::type bigger_integral;
    constexpr static bool fp = std::is_floating_point<T>::value || std::is_floating_point<TS>::value;
    constexpr static bool have_signed = !fp && (std::is_signed<T>::value || std::is_signed<TS>::value);

    typedef typename std::conditional<
        fp,
        typename std::common_type<T, TS>::type,
        typename mk_signed<bigger_integral, have_signed>::type
    >::type type;
};


template<class T>
inline T magnitude(const sf::Vector2<T> &vector) {
    return std::sqrt(std::pow(vector.x, 2) + std::pow(vector.y, 2));
}


template<class T>
inline sf::Vector2<T> norm(const sf::Vector2<T> &vector) {
    return sf::Vector2<T>{vector.x / magnitude(vector), vector.y / magnitude(vector)};
}


template<class T, class U>
inline typename best_common_numeric_type<T, U>::type dot(const sf::Vector2<T> &v1, const sf::Vector2<U> &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}


// multiplica um vector2 por um scalar
template<
    class U,
    class T,
    class Ret = typename best_common_numeric_type<T, U>::type,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type

>
inline sf::Vector2<Ret> operator*(const sf::Vector2<U> vector, T scalar) {
    return sf::Vector2<Ret>{vector.x * scalar, vector.y * scalar};
};


template<
    class T,
    class U,
    class Ret = typename best_common_numeric_type<T, U>::type,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
inline sf::Vector2<Ret> operator+(const sf::Vector2<T> &v1, const sf::Vector2<U> &v2) {
    return sf::Vector2<Ret>{v1.x + v2.x, v1.y + v2.y};
};


template<
    class T,
    class U,
    class Ret = typename best_common_numeric_type<T, U>::type,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
inline sf::Vector2<Ret> operator-(const sf::Vector2<T> &v1, const sf::Vector2<U> &v2) {
    return sf::Vector2<Ret>{v1.x - v2.x, v1.y - v2.y};
};


template <typename T> inline constexpr
int signum(T x, std::false_type is_signed) {
    return T(0) < x;
}

template <typename T> inline constexpr
int signum(T x, std::true_type is_signed) {
    return (T(0) < x) - (x < T(0));
}

template <typename T> inline constexpr
int signum(T x) {
    return signum(x, std::is_signed<T>());
}


/**
 * Retorna a direção em que um objeto precisa percorrer para interceptar um alvo na posição
 *`targetPos` com a velocidade definida por `targetVelocity`.
 * Este código só funciona bem se objectSpeed > targetVelocity
 * algoritmo tirado de: http://officialtwelve.blogspot.com.br/2015/08/projectile-interception.html
 * @tparam T tipo do vetor da posição original
 * @tparam U tipo do vetor da posição do alvo
 * @tparam V tipo do vetor de velocidade do alvo
 * @param originPos posição daonde o objeto está sendo "lançado"
 * @param targetPos posição atual do alvo
 * @param targetVelocity velocidade do alvo
 * @param objectSpeed velocidade do objeto
 * @return angulo da direção em que objeto precisa ser lançado
 */
template<class T,
    class U,
    class V,
    class Ret = typename best_common_numeric_type<T, U, V>::type>
inline Ret
interceptionAngle(sf::Vector2<T> originPos, sf::Vector2<U> targetPos, sf::Vector2<V> targetVelocity, int objectSpeed) {


    auto relativePos = originPos - targetPos;

    auto theta = std::atan2(targetVelocity.y, targetVelocity.x) - std::atan2(relativePos.y, relativePos.x);

    auto a = std::pow(magnitude(targetVelocity), 2) - std::pow(objectSpeed, 2);

    auto b = -2 * std::cos(theta) * magnitude(relativePos) * magnitude(targetVelocity);

    auto c = std::pow(magnitude(relativePos), 2);

    auto delta = std::sqrt((b * b) - (4 * a * c));

    auto t = -(b + delta) / (2 * a);

    if (std::abs(a) <= 0.000000000000001) {
        t = -c / (2 * b);
    }

    auto prediction = targetPos + targetVelocity * t;
    auto diff = prediction - originPos;

    Ret angleToShoot = std::atan2(diff.y, diff.x);
    return angleToShoot;

}


inline sf::Vector2f getMouseDirectionFromPosition(const sf::Vector2f &pos, sf::RenderWindow *screen) {

    auto mouse = screen->mapPixelToCoords(sf::Mouse::getPosition(*screen));
    auto playerPosition = pos;
    auto angleX = mouse.x - playerPosition.x;
    auto angleY = mouse.y - playerPosition.y;
    auto vl = std::sqrt(angleX * angleX + angleY * angleY);
    return sf::Vector2f{angleX / vl, angleY / vl};

}


inline sf::FloatRect calcViewRect(const sf::View &view) {
    sf::FloatRect viewRect;
    viewRect.left = view.getCenter().x - (view.getSize().x / 2.f);
    viewRect.top = view.getCenter().y - (view.getSize().y / 2.f);
    viewRect.width = view.getSize().x;
    viewRect.height = view.getSize().y;
    return viewRect;
}


inline sf::Vector3f getManifold(
    const sf::FloatRect &overlap,
    const sf::Vector2f &collisionNormal
) {
    sf::Vector3f manifold;
    if (overlap.width < overlap.height) {
        manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
        manifold.z = overlap.width;
    } else {
        manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
        manifold.z = overlap.height;
    }

    return manifold;
}
