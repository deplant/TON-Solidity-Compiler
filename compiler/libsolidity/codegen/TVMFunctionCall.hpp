/*
 * Copyright 2018-2022 TON DEV SOLUTIONS LTD.
 *
 * Licensed under the  terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the  GNU General Public License for more details at: https://www.gnu.org/licenses/gpl-3.0.html
 */
/**
 * @author TON Labs <connect@tonlabs.io>
 * @date 2019
 * Function call compiler for TVM
 */

#pragma once

#include <variant>

namespace solidity::frontend {

class TVMExpressionCompiler;
class StackPusher;

class FunctionCallCompiler {
public:
	FunctionCallCompiler(
		StackPusher& m_pusher,
		FunctionCall const& _functionCall,
		bool isCurrentResultNeeded
	);
	void structConstructorCall();
	void compile();

protected:
	static void arrayPush(StackPusher& pusher, Type const* arrayBaseType, DataType dataType);
	bool checkForMappingOrCurrenciesMethods();
	void mappingDelMinOrMax(bool isDelMin);
	void mappingGetSet();
	void mappingMinMaxMethod(bool isMin);
	void mappingPrevNextMethods();
	void mappingKeysOrValues(bool areKeys);
	void mappingEmpty();
	bool structMethodCall();
	void superFunctionCall(MemberAccess const& _node);
	void typeTypeMethods(MemberAccess const& _node);
	bool libraryCall(MemberAccess const& ma);
	bool checkForTvmDeployMethods(MemberAccess const& _node, Type::Category category);
	void tvmBuildIntMsg();
	void tvmBuildDataInit();
	void tvmBuildMsgMethod();
	void sliceMethods(MemberAccess const& _node);
	void arrayMethods(MemberAccess const& _node);
	bool checkForOptionalMethods(MemberAccess const& _node);
	bool checkForTvmBuilderMethods(MemberAccess const& _node, Type::Category category);
	bool checkForTvmVectorMethods(MemberAccess const& _node, Type::Category category);
	void cellMethods(MemberAccess const& _node);
	void variantMethods(MemberAccess const& _node);
	void addressMethod();
	bool checkForTvmConfigParamFunction(MemberAccess const& _node);
	bool checkForTvmSendFunction(MemberAccess const& _node);
	void msgFunction(MemberAccess const& _node);
	void rndFunction(MemberAccess const& _node);
	void goshFunction();
	bool checkForTvmFunction(MemberAccess const& _node);
	void abiFunction();
	void mathFunction(MemberAccess const& _node);
	bool checkBaseContractCall(MemberAccess const& _node, Type::Category category);
	bool checkAddressThis();
	void typeConversion();
	bool checkLocalFunctionOrLibCall(const Identifier* identifier);
	bool checkSolidityUnits();
	bool checkLocalFunctionOrLibCallOrFuncVarCall();
	bool checkNewExpression();
	void creatArrayWithDefaultValue();
public:
	void honestArrayCreation(bool onlyDict);
protected:
	bool createNewContract();
	void deployNewContract(
		const std::variant<int8_t, std::function<void()>>& wid,
		const std::variant<bigint, std::function<void()>>& value,
		const std::variant<bool, std::function<void()>>& pushBounce,
		const std::function<void()>& pushCurrency,
		const std::function<void(int builderSize)>& appendBody,
		const std::function<void()>& pushSendrawmsgFlag,
		const int argQty
	);
	void checkStateInit();

	enum StateInitMembers {
		SplitDepth,
		Special,
		Code,
		Data,
		Library
	};
	void buildStateInit(std::map<StateInitMembers, std::function<void()>> exprs);
	std::function<void()> generateDataSection(
		const std::function<void()>& pushKey,
		Expression const* vars,
		ContractType const* ct
	);
	bool checkRemoteMethodCall(FunctionCall const &_functionCall);
	void checkExtMsgSend();
	std::string getDefaultMsgValue();
	static const FunctionDefinition* getRemoteFunctionDefinition(const MemberAccess* memberAccess);
	void acceptExprOrPushFunctionId(Expression const* onerrorid);
	void generateExtInboundMsg(
		bool addSignature,
		const Expression * destination,
		const Expression *pubkey,
		const Expression *expire,
		const Expression *time,
		const Expression *callbackid,
		const Expression *onerrorid,
		const Expression *stateInit,
        const Expression *signBoxHandle,
		const CallableDeclaration *functionDefinition,
		const ast_vec<Expression const>& arguments
	);


	void pushArgs(bool reversed = false, bool doConvert = true);
	void pushArgAndConvert(int index, const std::string& name = "");
	void pushExprAndConvert(const Expression* expr, Type const* targetType);
	void acceptExpr(const Expression* expr);
	void compileLog();
	Expression const* findOption(const std::string& name);
	void cellBitRefQty(bool forCell = true);

private:
	StackPusher& m_pusher;
	TVMExpressionCompiler m_exprCompiler;
	FunctionCall const& m_functionCall;
	std::vector<ASTPointer<Expression const>> m_arguments;
	FunctionType const* m_funcType{};
	Type const* m_retType{};
	bool m_isCurrentResultNeeded{};
	std::vector<ASTPointer<ASTString>> const& m_names;
};

}	// solidity
