#include <iostream>
#include <vector>
#include <functional>
#include <cstring>
#include <cstdio>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

typedef std::pair<int, int> pii;

void init(int);
pii decide(pii);

namespace hex_judge {

    constexpr char serv_ip[] = "140.112.30.35";
    constexpr int serv_port = 4434;

    std::vector<int> mp;
    std::vector<int> ds[2];
    int find(std::vector<int> &x, int i) {
        if (x[i] == i) return i;
        else return x[i] = find(x, x[i]);
    }
    void unite(std::vector<int> &x, int a, int b) {
        x[find(x, b)] = find(x, a);
    }
    void unitep(int n, pii a, pii b, bool player) {
        int i = a.first * n + a.second;
        int j = -1;
        if (b.first >= 0 && b.first < n && b.second >= 0 && b.second < n) {
            j = b.first * n + b.second;
        } else if (player == 0) {
            if (b.first >= n) j = n * n + 1;
            else if (b.first < 0) j = n * n;
        } else {
            if (b.second >= n) j = n * n + 1;
            else if (b.second < 0) j = n * n;
        }
        if (j == -1) return;
        if (j < n * n && mp[j] != player) return;
        unite(ds[player], i, j);
    }
    void invalid(pii p) {
        std::cerr << "invalid point: (" << p.first << ", " << p.second << ")" << std::endl;
        throw;
    }
    void play(int n, pii p, bool player) {
        int i = p.first * n + p.second;
        if (!(p.first >= 0 && p.first < n && p.second >= 0 && p.second < n))
            invalid(p);
        if (mp[i] != -1) invalid(p);
        mp[i] = player;
        unitep(n, p, {p.first - 1, p.second}, player);
        unitep(n, p, {p.first + 1, p.second}, player);
        unitep(n, p, {p.first, p.second - 1}, player);
        unitep(n, p, {p.first, p.second + 1}, player);
        unitep(n, p, {p.first - 1, p.second + 1}, player);
        unitep(n, p, {p.first + 1, p.second - 1}, player);
    }
    void print(int n) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j)
                std::cout << " ";
            for (int j = 0; j < n; ++j) {
                int st = mp[i * n + j];
                if (st == 0) std::cout << "O ";
                else if (st == 1) std::cout << "X ";
                else std::cout << "_ ";
            }
            std::cout << std::endl;
        }
    }

    int sockfd;
    void socket_init() {
        #ifdef _WIN32
        WSADATA wsadata;
        if (WSAStartup(MAKEWORD(1, 1), &wsadata) == SOCKET_ERROR) {
            std::cerr << "WSAStartup() fail" << std::endl;
            exit(1);
        }
        #endif

        if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
            std::cerr << "socket() fail" << std::endl;
            exit(1);
        }

        sockaddr_in serveraddr;
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(serv_port);
        serveraddr.sin_addr.s_addr=inet_addr(serv_ip);

        if (connect(sockfd, (sockaddr*)&serveraddr ,sizeof(serveraddr)) == -1) {
            std::cerr << "connect() fail" << std::endl;
            exit(1);
        }
    }
    void socket_fin() {
        #ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
        #endif
    }
    void socket_read(char *buf, int maxlen) {
        int n;
        n = (int)recv(sockfd, buf, maxlen, 0);
        if (n < 0) {
            perror("read");
            std::cerr << "read() fail" << std::endl;
            exit(1);
        }
        buf[n] = 0;
    }
    void socket_send(char *buf) {
        send(sockfd, buf, strlen(buf), 0);
    }
    void judge_init(int n) {
        char buf[20];
        sprintf(buf, "init %d\n", n);
        socket_send(buf);
    }
    pii judge_decide(pii p) {
        char buf[40];
        sprintf(buf, "decide %d %d\n", p.first, p.second);
        socket_send(buf);
        int x, y;
        socket_read(buf, sizeof buf);
        sscanf(buf, "%d %d", &x, &y);
        return {x, y};
    }

}

int main() {
    using namespace hex_judge;
    socket_init();

    int num_rounds;
    std::cerr << "Please input the number of rounds: ";
    std::cin >> num_rounds;
    int num_win_round = 0;
    for (int round = 0; round < num_rounds; ++round) {
        int n;
        std::cerr << "Please input the size of the board: ";
        std::cin >> n;
        std::cerr << "Start playing with a " << n << "x" << n << " board." << std::endl;
        mp.clear();
        mp.resize(n * n, -1);
        ds[0].resize(n * n + 2);
        ds[1].resize(n * n + 2);
        for (int i = 0; i < n * n + 2; ++i)
            ds[0][i] = ds[1][i] = i;
        init(n);
        judge_init(n);
        pii last_move = {-1, -1};
        bool winner;
        while (true) {
            last_move = decide(last_move);
            std::cerr << "You played: (" << last_move.first << ", " << last_move.second << ")" << std::endl;
            play(n, last_move, 0);
            print(n);
            if (find(ds[0], n * n) == find(ds[0], n * n + 1)) {
                winner = 0;
                std::cerr << "You won!" << std::endl;
                print(n);
                break;
            }
            last_move = judge_decide(last_move);
            std::cerr << "Judge played: (" << last_move.first << ", " << last_move.second << ")" << std::endl;
            play(n, last_move, 1);
            print(n);
            if (find(ds[1], n * n) == find(ds[1], n * n + 1)) {
                winner = 1;
                std::cerr << "You lost!" << std::endl;
                print(n);
                break;
            }
        }
        if (winner == 0) ++num_win_round;
    }
    std::cout << "You won " << num_win_round << " out of " << num_rounds << " rounds" << std::endl;

    socket_fin();
    return 0;
}
