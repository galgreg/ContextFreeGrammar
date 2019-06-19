#include <chrono>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>

//-------------------- Type aliases -------------------------------
using std::vector;
using std::string;
using std::set;
using VariableSet = set<string>;
using ParsingCell = vector<VariableSet>;
using ParsingColumn = vector<ParsingCell>;
using ParsingTable = vector<ParsingColumn>;
using Clock = std::chrono::high_resolution_clock;
using TimeMeasure = std::chrono::duration<double>;
using RulesMap = std::unordered_multimap<string, string>;
using SetsPair = std::pair<set<string>, set<string>>;

//--------------- Auxiliary I/O functions -----------------------------
vector<string> split(const string &text, const string &delimiter);
void printAnswer(const vector<bool> &verdicts);
//--------------- Algorithm functions ---------------------------
bool belongsToLanguage(
		const RulesMap &grammarRules,
		const string &word);
VariableSet getCellContent(const ParsingCell &cell);
set<string> merge(const SetsPair &pairOfSets);
set<string> getVariablesForRules(
		const RulesMap &grammarRules,
		const set<string> &rulesToCheck);
void prepareCellForFutureUsage(ParsingCell &cell);
bool isParsable(unsigned wordSize, const ParsingTable &parsingTable);
// ----------- Debug purpose functions ---------------------------
void print(const ParsingTable &parsingTable);
void print(const RulesMap &grammarRules);
void printWhichCellInTable(unsigned i, unsigned j);
void print(
		const set<string> &printedSet,
		string setName,
		unsigned i,
		unsigned j,
		unsigned k);
