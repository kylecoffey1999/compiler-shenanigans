#include <array>
#include <boost/dynamic_bitset.hpp>
#include <boost/variant2/variant.hpp>
#include <fmt/format.h>
#include <iostream>
#include <memory>
#include <stack>
#include <string_view>
#include <vector>

#define DEBUG

class BitSet {
  using _internal_type = boost::dynamic_bitset<>;
  _internal_type _set;
  bool _isComplement = false;

public:
  class const_iterator {
    const BitSet &_set;
    std::size_t _pos;

  public:
    const_iterator(const BitSet &set) : _set{set}, _pos{0} {}
    const_iterator(const BitSet &set, std::size_t pos) : _set{set}, _pos{pos} {}
    _internal_type::const_reference operator*() const {
      return _set._set[_pos];
    }
    const_iterator &operator++() {
      while (_pos < _set._set.size() && !_set._set[++_pos]) {
      }
      return *this;
    }
    bool at_end() const { return _pos == _set._set.size(); }
  };

  void set(std::size_t bit) {
    _set.resize(std::max(_set.size(), bit), _isComplement);
    _set.set(bit);
  }
  void complement() {
    _set = ~_set;
    _isComplement = !_isComplement;
  }
  bool get(std::size_t bit) { return _set[bit]; }
  const_iterator begin() { return const_iterator{*this}; }
  const_iterator end() { return const_iterator{*this, _set.size()}; }
};

static constexpr char edgeEmpty = -3;
static constexpr char edgeCharacterClass = -2;
static constexpr char edgeEpsilon = -1;

static constexpr int anchorNone = 0;
static constexpr int anchorLineStart = 1 << 0;
static constexpr int anchorLineEnd = 1 << 1;
static constexpr int anchorBoth = anchorLineStart | anchorLineEnd;

struct NfaNode {
  std::array<std::size_t, 2> next;
  char edge;
  boost::dynamic_bitset<> bitset;
  int anchor;
  int index;
  NfaNode(int index)
      : next{SIZE_MAX, SIZE_MAX}, edge{0}, anchor{0}, index{index} {}
};

struct Nfa {
  std::vector<NfaNode> nodes;
  std::size_t startState;
};

static boost::variant2::variant<Nfa, std::string>
thompson(std::string_view input);
static std::ostream &operator<<(std::ostream &os, const Nfa &nfa);

struct ParserState {
#ifdef DEBUG
  std::size_t indentLevel;
  void enter(std::string_view functionName) {
    fmt::print("{:{}}enter {}\n", indentLevel++, "", functionName);
  }
  void leave(std::string_view functionName) {
    fmt::print("{:{}}leave {}\n", --indentLevel, "", functionName);
  }
#else
  void enter(std::string_view functionName) {}
  void leave(std::string_view functionName) {}
#endif

private:
  static std::vector<NfaNode> nfaStates;
  static std::stack<std::size_t> discarded_nfa_states;

  std::size_t allocateNfaNode() {
    if (!discarded_nfa_states.empty()) {
      std::size_t n = discarded_nfa_states.top();
      discarded_nfa_states.pop();
      nfaStates[n].index = n;
      return n;
    }

    nfaStates.emplace_back(nfaStates.size());
    return nfaStates.size() - 1;
  }

  void discardNfaNode(std::size_t index) {
    NfaNode *node = &nfaStates[index];
    node->index = -1;
    node->bitset.clear();
    node->anchor = anchorNone;
    node->edge = edgeEmpty;
    discarded_nfa_states.emplace(node->index);
  }

  using RegexToken = int;
  static constexpr RegexToken tokEos = 1;
  static constexpr RegexToken tokDot = 2;
  static constexpr RegexToken tokCarat = 3;
  static constexpr RegexToken tokDollar = 4;
  static constexpr RegexToken tokRightBracket = 5;
  static constexpr RegexToken tokLeftBracket = 6;
  static constexpr RegexToken tokRightParen = 7;
  static constexpr RegexToken tokLeftParen = 8;
  static constexpr RegexToken tokStar = 9;
  static constexpr RegexToken tokDash = 10;
  static constexpr RegexToken tokLiteral = 11;
  static constexpr RegexToken tokQuestionMark = 12;
  static constexpr RegexToken tokPipe = 13;
  static constexpr RegexToken tokPlus = 14;

  static constexpr std::array<RegexToken, 128> Tokmap{
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokDollar,      tokLiteral, tokLiteral,      tokLiteral,
      tokLeftParen, tokRightParen,  tokStar,    tokPlus,         tokLiteral,
      tokDash,      tokDot,         tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokQuestionMark, tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLeftBracket, tokLiteral, tokRightBracket, tokCarat,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokLiteral,
      tokLiteral,   tokLiteral,     tokLiteral, tokLiteral,      tokPipe,
      tokLiteral,   tokLiteral,
  };

  const char *input;
  std::string start_of_input;
  RegexToken currentToken;
  char lexeme;

  static constexpr bool IS_HEX_DIGIT(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
  }

  static char hex2bin(char c) {
    if (std::isdigit(c)) {
      return c - '0';
    }
    return (std::toupper(c) - 'A') & 0xF;
  }

  static constexpr bool IS_OCT_DIGIT(char c) { return c >= '0' && c <= '7'; }

  static char oct2bin(char c) { return (c - '0') & 0x7; }

  static char esc(const char **s) {
    if ((*s)[0] != '\\') {
      char rval = **s;
      ++(*s);
      return rval;
    }
    ++(*s);
    char rval;
    switch (std::toupper((*s)[0])) {
    case '\0':
      rval = '\\';
      break;
    case 'B':
      rval = '\b';
      break;
    case 'F':
      rval = '\f';
      break;
    case 'N':
      rval = '\n';
      break;
    case 'R':
      rval = '\r';
      break;
    case 'T':
      rval = '\t';
      break;
    case 'E':
      rval = '\x1b';
      break;
    case '^':
      ++(*s);
      rval = (*s)[0];
      rval = std::toupper(rval) - '@';
      break;
    case 'X':
      rval = 0;
      ++(*s);
      if (IS_HEX_DIGIT((*s)[0])) {
        rval = hex2bin((*s)[0]);
        ++(*s);
      }
      if (IS_HEX_DIGIT((*s)[0])) {
        rval <<= 4;
        rval |= hex2bin((*s)[0]);
        ++(*s);
      }
      if (IS_HEX_DIGIT((*s)[0])) {
        rval <<= 4;
        rval |= hex2bin((*s)[0]);
        ++(*s);
      }
      --(*s);
      break;
    default:
      if (!IS_OCT_DIGIT((*s)[0])) {
        rval = (*s)[0];
      } else {
        ++(*s);
        rval = oct2bin((*s)[0]);
        ++(*s);
        if (IS_OCT_DIGIT((*s)[0])) {
          rval <<= 3;
          rval |= oct2bin((*s)[0]);
          ++(*s);
        }
        if (IS_OCT_DIGIT((*s)[0])) {
          rval <<= 3;
          rval |= oct2bin((*s)[0]);
          ++(*s);
        }
        --(*s);
        break;
      }
    }
    ++(*s);
    return rval;
  }

  char advance() {
    static bool inQuote = false;
    static std::stack<const char *> stack;
    if (currentToken == tokEos && inQuote) {
      throw std::runtime_error{"Newline in quoted string"};
    }
    if (input[0] == '"') {
      inQuote = !inQuote;
      ++input;
      if (input[0] == '\0') {
        currentToken = tokEos;
        lexeme = '\0';
        return lexeme;
      }
    }
    bool sawEsc = input[0] == '\\';
    if (!inQuote) {
      lexeme = esc(&input);
    } else {
      if (sawEsc && input[1] == '"') {
        input += 2;
        lexeme = '"';
      } else {
        lexeme = input[0];
        ++input;
      }
    }
    currentToken = (inQuote || sawEsc) ? tokLiteral : Tokmap[lexeme];
    return currentToken;
  }

  void catExpr(std::size_t *, std::size_t *);
  static void dodash(boost::dynamic_bitset<> *);
  void expr(std::size_t *, std::size_t *);
  void factor(std::size_t *, std::size_t *);
  bool firstInCat(RegexToken);
  Nfa machine();
  std::size_t rule();
  void term(std::size_t *, std::size_t *);
};

Nfa ParserState::machine() {
  enter("machine");
  std::size_t p = allocateNfaNode();
  NfaNode *pn = &nfaStates[p];
  std::size_t start = p;
  pn->next[0] = rule();
  while (currentToken != tokEos) {
    pn->next[1] = allocateNfaNode();
    p = pn->next[1];
    pn = &nfaStates[p];
    pn->next[0] = rule();
  }
  leave("machine");
  return Nfa{.nodes = nfaStates, .startState = start};
}

std::size_t ParserState::rule() {
  std::size_t start;
  std::size_t end;
  int anchor = anchorNone;
  enter("rule");
  if (currentToken == tokCarat) {
    start = allocateNfaNode();
    nfaStates[start].edge = '\n';
    anchor |= anchorLineStart;
    advance();
    expr(&nfaStates[start].next[0], &end);
  } else {
    expr(&start, &end);
  }
  if (currentToken == tokDollar) {
    advance();
    nfaStates[end].next[0] = allocateNfaNode();
    nfaStates[end].edge = edgeCharacterClass;
    nfaStates[end].bitset.resize(std::max('\r', '\n'));
    nfaStates[end].bitset.set('\n');
    nfaStates[end].bitset.set('\r');
  }
}

int main() { return 0; }
