#include <chrono>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>

//-------------------- Type aliases -------------------------------
using std::vector;
using std::string;
using std::set;
using ParsingTable = vector<vector<set<string>>>;
using ParsingColumn = vector<set<string>>;
using Clock = std::chrono::high_resolution_clock;
using TimeMeasure = std::chrono::duration<double>;
using RulesMap = std::unordered_multimap<string, string>;
using SetsPair = std::pair<set<string>, set<string>>;

//--------------- Function declarations ---------------------------
vector<string> split(const string &text, const string &delimiter);
bool belongsToLanguage(
		const RulesMap &grammarRules,
		const string &word);
void printAnswer(const vector<bool> &verdicts);
bool isParsable(unsigned wordSize, const ParsingTable &parsingTable);
set<string> merge(const SetsPair &pairOfSets);
set<string> getVariablesForRules(
		const RulesMap &grammarRules,
		const set<string> &rulesToCheck);
void print(const ParsingTable &parsingTable);
void print(const RulesMap &grammarRules);
void printWhichCellInTable(unsigned i, unsigned j);
void print(
		const set<string> &printedSet,
		string setName,
		unsigned i,
		unsigned j,
		unsigned k);
