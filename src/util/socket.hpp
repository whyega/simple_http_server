#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace util {
/**
 * @class Socket
 * @brief Кроссплатформенный класс для работы с сокетами (На данный момент
 * поддерживается только TCP сервер).
 */
class Socket {
 private:
  using socket_handle_t = std::uintptr_t;
  socket_handle_t socket_;

 public:
  enum class family_t { inet = 2, inet6 = 6 };
  enum class specification_t { stream = 1 };
  enum class protocol_t { tcp, udp };

  Socket() = delete;

  /**
   * @brief Конструктор класса Socket.
   *
   * @param family Семейство адресов (например, IPv4 или IPv6).
   * @param specification Тип сокета (например, потоковый или датаграммный).
   * @param protocol Протокол (например, TCP или UDP).
   * @throws std::runtime_error Если не удалось инициализировать сокет.
   */
  Socket(family_t family, specification_t specification, protocol_t protocol);

  /**
   * @brief Деструктор класса Socket.
   *
   * Закрывает сокет и освобождает ресурсы.
   */
  ~Socket();
  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;

  /**
   * @brief Конструктор перемещения.
   *
   * @param other Другой объект Socket, из которого перемещаются ресурсы.
   */
  Socket(Socket&& other) noexcept;

  /**
   * @brief Оператор присваивания перемещением.
   *
   * @param other Другой объект Socket, из которого перемещаются ресурсы.
   * @return Socket& Ссылка на текущий объект.
   */
  Socket& operator=(Socket&& other) noexcept;

  /**
   * @brief Привязывает сокет к указанному адресу и порту.
   *
   * @param address IP-адрес для привязки.
   * @param port Порт для привязки.
   * @throws std::runtime_error Если привязка не удалась.
   */
  void Bind(const std::string& address, std::uint16_t port);

  /**
   * @brief Переводит сокет в режим прослушивания.
   *
   * @throws std::runtime_error Если прослушивание не удалось.
   */
  void Listen();

  /**
   * @brief Принимает входящее соединение.
   *
   * @return Socket Новый сокет для общения с клиентом.
   * @throws std::runtime_error Если принятие соединения не удалось.
   */
  Socket Accept();

  /**
   * @brief Читает данные из сокета.
   *
   * @param size Максимальное количество байт для чтения.
   * @return std::vector<std::uint8_t> Прочитанные данные.
   * @throws std::runtime_error Если чтение не удалось.
   */
  std::vector<std::uint8_t> Read(const std::size_t size);

  /**
   * @brief Записывает данные в сокет.
   *
   * @param data Данные для записи.
   * @throws std::runtime_error Если запись не удалась.
   */
  void Write(const std::vector<std::uint8_t>& data);
};
}  // namespace util
