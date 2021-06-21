#include <iostream>
#include <vector>
#include <map>
#include <limits>
using namespace std;
using Satoshi = uint64_t;
using Address = uint64_t;
//The amount of money created every block and given to the block's miner
#define MONEY_CREATED_FOR_THE_MINER_EACH_BLOCK 1000
#define MAX_ENTRIES_IN_BLOCK 10
#define SIG_LENGTH 32
struct Transaction
{
	//creator of transaction and sender of funds
	Address from;
	//transaction destination
	Address to;
	//amount of money to send to the destination
	Satoshi amount;
	//amount of fee is offered by the creator to the miner to add this
	//transaction to the blockchain
	Satoshi fee;
	//creation time, written by the transaction creator
	time_t creationTime;
	//signing {from,amount,to,fee,creation time}
	uint8_t signature[SIG_LENGTH];
};
struct Block
{
	//holds up to 10 transactions
	std::vector<Transaction> trans;
	//the miner that added the block to the blockchain
	Address miner;
	// the next block at the blockchain (is nullptr for the last block)
	Block* next;
};
class Blockchain
{
	//not null
	Block* m_firstBlock;
public:
	//assumption firstBlock is not null
	Blockchain(Block* firstBlock) : m_firstBlock(firstBlock) {}
	//Assumption: all the transaction in the blockchain ("firstBlock linkedlist") are valid
	//return whether or not the new transaction is valid, given this blockchain
	//No need to validate the crypto signature
	bool isValid(const Transaction& newTransaction) const
	{
		if (!newTransaction.from || !newTransaction.to || !newTransaction.fee || !newTransaction.creationTime)
			return false;

		if (newTransaction.from == newTransaction.to)
			return false;

		//find last block
		Block* lastBlock = m_firstBlock;
		while (lastBlock->next)
			lastBlock = lastBlock->next;

		//full block transaction can't be added
		if (lastBlock->trans.size() == MAX_ENTRIES_IN_BLOCK)
			return false;

		//check transaction signature uniqueness
		Block* curBlock = m_firstBlock;
		do
		{
			for (const auto& trans : curBlock->trans)
				if (!memcmp(trans.signature, newTransaction.signature, SIG_LENGTH))
					return false;
		} while (curBlock = curBlock->next);

		//calculate transaction amount validity from the genesis block
		curBlock = m_firstBlock;
		uint64_t wallet = 0;

		do
		{
			if (newTransaction.from == curBlock->miner)
				wallet += MONEY_CREATED_FOR_THE_MINER_EACH_BLOCK;

			for (const auto& trans : curBlock->trans)
			{
				//collect fees if creator's wallet is the miner
				if (newTransaction.from == curBlock->miner)
					wallet += trans.fee;

				//add or deduct amount from creator's wallet
				if (newTransaction.from == trans.to)
					wallet += trans.amount;
				else if (newTransaction.from == trans.from)
					wallet -= trans.amount;
			}
		} while (curBlock = curBlock->next);

		if (newTransaction.amount + newTransaction.fee < wallet)
			return false;

		return true;
	}
};
//int main()
//{
//	std::cout << "Hello Blockchain! Very nice to meet you! My name is 	main()" << std::endl;
//
//	Transaction a1{ 1,2,3,4,5,{1,1,1,1,1,1,1,1} };
//	Transaction a2{ 1,2,3,4,6,{1,1,1,1,1,1,1,5} };
//	Transaction a3{ 1,2,3,4,7,{1,1,1,1,1,1,1,3} };
//	Transaction b1{ 1,2,3,4,8,{1,1,1,1,1,1,1,4} };
//	Transaction b2{ 1,2,3,4,9,{1,1,1,1,1,1,1,5} };
//	Block blk1, blk2;
//	blk1.miner = 123;
//	blk1.next = nullptr;
//	blk1.trans.push_back(a1);
//	blk1.trans.push_back(a2);
//	blk1.trans.push_back(a3);
//
//	blk2.miner = 234;
//	blk2.next = nullptr;
//	blk2.trans.push_back(b1);
//
//
//	blk1.next = &blk2;
//
//	Blockchain blkchain(&blk1);
//	cout << blkchain.isValid(b2);
//
//	return 0;
//}