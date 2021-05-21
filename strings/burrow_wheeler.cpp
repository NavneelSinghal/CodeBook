// burrow wheeler transform - find the string consisting of the last elements of
// the sorted rotated arrays

// inverse burrow wheeler transform

string s;
read_str(s);
int n = s.size();
vector<int> nextPosition;
vector<vector<int>> positions(27);

for (int i = 0; i < n; ++i) positions[max(0, s[i] - 'a' + 1)].push_back(i);

for (int i = 0; i < 27; ++i)
    for (auto position : positions[i]) nextPosition.push_back(position);

int position = -1;
for (int i = 0; i < n; ++i) {
    if (s[i] == '#') {
        position = i;
        break;
    }
}

assert(~position);

for (int i = 1; i < n; ++i) {
    position = nextPosition[position];
    write_char(s[position]);
}

write_char('\n');
