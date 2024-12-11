#ifndef UTILS_H
#define UTILS_H

int is_valid_flag(const char *arg);
const char *get_filename(int argc, char **argv);
void print_score(int score);
void print_no_bacon();
void print_actor_not_found(const char *actor);

#endif
