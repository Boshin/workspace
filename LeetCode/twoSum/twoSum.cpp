#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

class Solution 
{
	public:
#if 0
		std::vector<int> twoSum(std::vector<int> &numbers, int target) 
		{
			std::vector<int> targetVec;
			int maxSize = numbers.size();

			for(int i = 1; i < maxSize; ++i)
			{
				//if(0 == numbers[i])
					//continue;

				for(int j = 1; j < i; ++j)
				{
					//if(0 == numbers[j] || numbers[i] >= numbers[j])
						//continue;

					if(target == (numbers[i] + numbers[j]))
					{
						std::cout << "i: " << i << ", j: " << j <<"\n";
						targetVec.push_back(i);	
						targetVec.push_back(j);	

						return targetVec;
					}
				}
			}

			return targetVec;
		}
#else
		std::vector<int> twoSum(std::vector<int> &numbers, int target) 
		{
			std::vector<int> targetVec;
			int maxSize = numbers.size();
			std::unordered_map<int,int> hashMap;
			
			for(int i = 0; i < maxSize; ++i)	
			{
				int targetNum = target - numbers[i];

				if(hashMap.find(targetNum) != hashMap.end())
				{
					targetVec.push_back(hashMap[targetNum] + 1);
					targetVec.push_back(i + 1);

					return targetVec;
				}

				hashMap[numbers[i]] = i;
			}

			return targetVec;
		}
#endif
};

int main(int argc, char *argv[])
{
	int arr[] = {6 ,1 , 2, 3, 4, 5, 0};
	//int arr[] = {0, 4, 3, 0};
	//int arr[] = {-3, 4, 3, 90};
	std::vector<int> tmpVec;
	std::vector<int> resVec;
	Solution sol;

	for(unsigned int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
	{
		tmpVec.push_back(arr[i]);
	}

	resVec = sol.twoSum(tmpVec, 5);

	for(unsigned int i = 0; i < resVec.size(); ++i)
	{
		std::cout << resVec[i] << " ";
	}

	std::cout << "\n";
	
	return 0;
}
