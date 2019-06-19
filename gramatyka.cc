#include "gramatyka.h"
#include <iostream>
#include <sstream>
#include <omp.h>
#define DEBUG_PRINT_GRAMMAR_RULES 0
#define DEBUG_PRINT_BELONGS_TO_LANGUAGE 0
#define DEBUG_PRINT_PARSING_TABLE 0
//--------------- main function ---------------------------
int main() {
	string inputLine;
	RulesMap grammarRules;
	vector<string> words;
	
	while (std::getline(std::cin, inputLine) && !inputLine.empty()) {		
		auto keyAndValues = split(inputLine, "-> |");
		unsigned i = 1;
		auto keyAndValue = std::make_pair(
				keyAndValues.at(0),
				keyAndValues.at(0));
		grammarRules.insert(keyAndValue);
		
		while (i < keyAndValues.size()) {
			keyAndValue = std::make_pair(
					keyAndValues.at(i),
					keyAndValues.at(0));
			grammarRules.insert(keyAndValue);
			++i;
		}
	}
	#if DEBUG_PRINT_GRAMMAR_RULES == 1
	print(grammarRules);
	#endif
	
	while (std::getline(std::cin, inputLine) && inputLine != "0") {
		words.push_back(inputLine);
	}
	const size_t wordsCount = words.size();
	vector<bool> verdicts(wordsCount);
	
	auto beginTime = Clock::now();
	for(size_t i = 0; i < wordsCount; ++i) {
		verdicts[i] = belongsToLanguage(grammarRules, words[i]);
	}
	auto endTime = Clock::now();
	
	printAnswer(verdicts);
	TimeMeasure executionTime = endTime - beginTime;
	std::cout << "Execution time: " << executionTime.count()
			<< " seconds." << std::endl;
	return EXIT_SUCCESS;
}
//--------------- Algorithm functions ---------------------------
bool belongsToLanguage(
		const RulesMap &grammarRules,
		const string &word) {
	const unsigned wordSize = word.size();
	
	ParsingTable parsingTable(
			wordSize,
			ParsingColumn(wordSize));
	
	#pragma omp parallel if(wordSize > 100) default(none) \
	shared(word, grammarRules, parsingTable)
	{
		#pragma omp for nowait
		for (unsigned i = 0; i < wordSize; ++i) {
			string temp(1, word[i]);
			auto variablesRange = std::move(grammarRules.equal_range(temp));
			set<string> tempSet;
			for (auto it = variablesRange.first; it != variablesRange.second; ++it) {
				tempSet.insert(it->second);
			}
			parsingTable[i][i].push_back(std::move(tempSet));
		}
	}
	
	for (unsigned r = 1; r < wordSize; ++r) {
		for(unsigned i = 0; i < wordSize-r; ++i) {
			const unsigned j = i + r;
			auto &currentCell = parsingTable[i][j];
			const unsigned numberOfVariableSets = r;
			currentCell.resize(numberOfVariableSets);
			#if DEBUG_PRINT_BELONGS_TO_LANGUAGE == 1
			printWhichCellInTable(i, j);
			#endif
			
			#pragma omp parallel if(wordSize > 100) default(none) \
			shared(parsingTable, grammarRules, currentCell, i)
			{
				#pragma omp for nowait
				for(unsigned k = i; k < j; ++k) {
					auto leftSubstringCell = parsingTable[i][k][0];
					auto rightSubstringCell = parsingTable[k+1][j][0];
					auto pairOfSets = std::make_pair(leftSubstringCell, rightSubstringCell);
					auto rulesToCheck = merge(pairOfSets);
					#if DEBUG_PRINT_BELONGS_TO_LANGUAGE == 1
					print(rulesToCheck, "rulesToCheck", i, j, k);
					#endif
					auto variableSet = getVariablesForRules(grammarRules, rulesToCheck);
					#if DEBUG_PRINT_BELONGS_TO_LANGUAGE == 1
					print(variableSet, "variableSet", i, j, k);
					#endif
					const unsigned index = k - i;
					currentCell[index].insert(variableSet.cbegin(), variableSet.cend());
				}
			}
			prepareCellForFutureUsage(currentCell);
		}
	}
	#if DEBUG_PRINT_PARSING_TABLE == 1
	print(parsingTable);
	#endif
	return isParsable(wordSize, parsingTable);
}

VariableSet getCellContent(const ParsingCell &cell) {
	VariableSet cellContent;
	for (const auto &item : cell) {
		cellContent.insert(item.cbegin(), item.cend());
	}
	return std::move(cellContent);
}

set<string> merge(const SetsPair &pairOfSets) {
	set<string> mergedSet;
	for (const auto &first : pairOfSets.first) {
		for (const auto &second : pairOfSets.second) {
			string temp = first + second;
			mergedSet.insert(temp);
		} 
	}
	return std::move(mergedSet);
}

set<string> getVariablesForRules(
		const RulesMap &grammarRules,
		const set<string> &rulesToCheck) {
	set<string> variables;
	
	for (const auto &rule : rulesToCheck) {
		const auto &it = grammarRules.find(rule);
		if (it != grammarRules.cend()) {
			variables.insert(it->second);
		}
	}
	return std::move(variables);
}

void prepareCellForFutureUsage(ParsingCell &cell) {
	VariableSet cellContent = getCellContent(cell);
	cell.clear();
	cell.push_back(cellContent);
}

bool isParsable(unsigned wordSize, const ParsingTable &parsingTable) {
	const auto &cellContent = parsingTable[0][wordSize-1][0];
		
	const auto it = cellContent.find("S");
	if (it != cellContent.cend()) {
		return true;
	}
	return false;
}
//--------------- Auxiliary I/O functions -----------------------------
vector<string> split(
		const string &text,
		const string &delimiters) {
	vector<string> tokens;
	string singleToken;
	std::istringstream tokenStream(text);
	
	while (tokenStream >> singleToken) {
		if (delimiters.find(singleToken) != string::npos) {
			continue;
		}
		tokens.push_back(singleToken);
	}
	return std::move(tokens);
}
void printAnswer(const vector<bool> &verdicts) {
	for (auto verdict : verdicts) {
		if (verdict) {
			std::cout << "TAK\n";
		} else {
			std::cout << "NIE\n";
		}
	}
}
//------------------------- Function for debug purpose! ----------------------------
void print(const ParsingTable &parsingTable) {
	std::cout << "Parsing table content for wordSize equals "
			<< parsingTable.size() << ":\n";
	for (unsigned i = 0; i != parsingTable.size(); ++i) {
		for (unsigned j = 0; j != parsingTable[i].size(); ++j) {
			const auto &cellContent = parsingTable[i][j][0];

			if (cellContent.cbegin()->empty()) {
				continue;
			} else {
				std::cout << "parsingTable[" << i << "][" << j << "]: ";
				for (const auto &item : cellContent) {
					std::cout << "\'" << item << "\' ";
				}
				std::cout << std::endl;
			}
		}
	}
}
void print(const RulesMap &grammarRules) {
	std::cout << "Grammar rules: " << std::endl;
	for (const auto &rule : grammarRules) {
		std::cout << "variable: " << rule.second << " , rule: " << rule.first << std::endl;
	}
}
void printWhichCellInTable(unsigned i, unsigned j) {
	std::cout << "parsingTable[" << i << "][" << j << "]:" << std::endl;
}
void print(
		const set<string> &printedSet,
		string setName,
		unsigned i,
		unsigned j,
		unsigned k) {
	std::cout << "\t" + setName + "[" << i
			<< "][" << k << "], [" << k+1
			<< "][" << j << "]: ";
	for (const auto &rule : printedSet) {
		std::cout << "\'"<< rule << "\' ";
	}
	std::cout << std::endl;
}
