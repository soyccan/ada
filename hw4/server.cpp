#include <iostream>
#include <cstring>
#include <vector>
#include <functional>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

typedef std::pair<int, int> pii;

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
bool play(int n, pii p, bool player) {
    int i = p.first * n + p.second;
    if (!(p.first >= 0 && p.first < n && p.second >= 0 && p.second < n))
        return false;
    if (mp[i] != -1) return false;
    mp[i] = player;
    unitep(n, p, {p.first - 1, p.second}, player);
    unitep(n, p, {p.first + 1, p.second}, player);
    unitep(n, p, {p.first, p.second - 1}, player);
    unitep(n, p, {p.first, p.second + 1}, player);
    unitep(n, p, {p.first - 1, p.second + 1}, player);
    unitep(n, p, {p.first + 1, p.second - 1}, player);
    return true;
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

int setup_childprocess(const char *path, FILE *&input, FILE *&output) {
    int in_fds[2], out_fds[2];
    if (pipe(in_fds) < 0) {
        perror("Failed to create pipe.");
        exit(1);
    }
    if (pipe(out_fds)) {
        perror("Failed to create pipe.");
        exit(1);
    }
    int pid = fork();
    if (pid < 0) {
        perror("Failed to fork.");
        exit(1);
    }
    if (pid == 0) {
        dup2(in_fds[0], STDIN_FILENO);
        close(in_fds[1]);
        dup2(out_fds[1], STDOUT_FILENO);
        close(out_fds[0]);
        if (execlp(path, path, NULL) < 0) {
            perror("Failed to exec.");
        }
    }
    input = fdopen(in_fds[1], "w");
    output = fdopen(out_fds[0], "r");
    return pid;
}

int player_pid[2];
FILE *player_input[2], *player_output[2];
pii decide(int player, pii last_move, const int timelimit) {
    if (player == 1) std::swap(last_move.first, last_move.second);
    fprintf(player_input[player], "decide %d %d\n", last_move.first, last_move.second);
    fflush(player_input[player]);

    pii res;

    struct timeval tv;
    tv.tv_sec = timelimit;
    tv.tv_usec = 0;
    fd_set readfds;
    FD_ZERO(&readfds);
    int fd = fileno(player_output[player]);
    FD_SET(fd, &readfds);
    select(fd + 1, &readfds, NULL, NULL, &tv);
    if (!FD_ISSET(fd, &readfds)) return pii(-1, -1);

    fscanf(player_output[player], "%d %d", &res.first, &res.second);
    if (player == 1) std::swap(res.first, res.second);
    return res;
}


int main(int argc, char *argv[]) {
    int num_rounds, n;
    if (argc >= 4) {
        num_rounds = atoi(argv[3]);
        n = atoi(argv[4]);
    } else {
        std::cerr << "Usage: " << argv[0]
            << " [Player1 Program] [Player2 Program] [Number of Rounds] [Size of the board]" << std::endl;
        exit(1);
    }

    int num_win_round = 0;
    for (int round = 0; round < num_rounds; ++round) {
        std::cerr << "===== Round #" << round + 1 << " =====" << std::endl;
        player_pid[0] = setup_childprocess(argv[1], player_input[0], player_output[0]);
        player_pid[1] = setup_childprocess(argv[2], player_input[1], player_output[1]);
        std::cerr << "Start playing with a " << n << "x" << n << " board." << std::endl;
        mp.clear();
        mp.resize(n * n, -1);
        ds[0].resize(n * n + 2);
        ds[1].resize(n * n + 2);
        for (int i = 0; i < n * n + 2; ++i)
            ds[0][i] = ds[1][i] = i;
        for (int i : {0, 1}) {
            fprintf(player_input[i], "init %d\n", n);
            fflush(player_input[i]);
        }
        pii last_move = {-1, -1};
        bool winner;
        bool first_move = true;
        while (true) {
            for (int i : {0, 1}) {
                last_move = decide(i, last_move, 3);
                if (last_move == pii(-1, -1)) {
                    winner = 1 - i;
                    std::cerr << "Player " << i + 1 << " timeout" << std::endl;
                    std::cerr << "Player " << (1 - i) + 1 << " wins!" << std::endl;
                    goto finish_game;
                }
                std::cerr << "Player" << i + 1 << " played: ("
                    << last_move.first << ", " << last_move.second << ")" << std::endl;
                if (first_move && last_move.first <= 6 && last_move.first >= 4
                    && last_move.second >= 4 && last_move.second <= 6) {
                    winner = 1 - i;
                    std::cerr << "Player " << i + 1 << " plays an invalid move" << std::endl;
                    std::cerr << "Player " << (1 - i) + 1 << " wins!" << std::endl;
                    goto finish_game;
                }
                first_move = false;
                if (!play(n, last_move, i)) {
                    winner = 1 - i;
                    std::cerr << "Player " << i + 1 << " plays an invalid move." << std::endl;
                    std::cerr << "Player " << (1 - i) + 1 << " wins!" << std::endl;
                    goto finish_game;
                }
                if (find(ds[i], n * n) == find(ds[i], n * n + 1)) {
                    winner = i;
                    std::cerr << "Player " << i + 1 << " wins!" << std::endl;
                    goto finish_game;
                }
            }
            print(n);
        }
        finish_game:
        std::cerr << std::endl;
        print(n);
        if (winner == 0) ++num_win_round;
        std::cout << "Score: " << num_win_round << " vs " << round + 1 - num_win_round << std::endl;
        std::cerr << std::endl;
        for (int i : {0, 1}) {
            kill(player_pid[i], SIGKILL);
            wait(NULL);
        }
    }
}
