#include "Animate.h"

Animate::Animate()
{
	
}

Animate::~Animate()
{
	/*bufferVal.clear();

	for (int i = 0; i < 4; ++i) buffer[i].clear();*/
}

void Animate::AddToBuffer(float& Val, float Start, float End, float Duration, bool Stop)
{
	bufferVal.push_back(&Val);
	buffer[0].push_back(Start);
	buffer[1].push_back(End);
	buffer[2].push_back(Duration);
	buffer[3].push_back(Stop);
}

bool Animate::Update(double dt)
{
	if (buffer[0].size() != 0 && buffer[0].size() < 1000)
		for (int i = 0; i < buffer[0].size(); ++i)
		{
		//	ANI_VALUE	 -= dt * ((Cur - Tar) / Dur)
			*bufferVal[i] -= dt * ((buffer[0][i] - buffer[1][i]) / buffer[2][i]);

			if (buffer[0][i] - buffer[1][i] <= 0)
			{
				if (*bufferVal[i] >= buffer[1][i])
				{
					*bufferVal[i] = buffer[1][i];
					bufferVal.erase(bufferVal.begin() + i);
					for (int j = 0; j < 4; ++j) buffer[j].erase(buffer[j].begin() + i);

					--i;
					continue;
				}
			}
			else
			{
				if (*bufferVal[i] <= buffer[1][i])
				{
					*bufferVal[i] = buffer[1][i];
					bufferVal.erase(bufferVal.begin() + i);
					for (int j = 0; j < 4; ++j) buffer[j].erase(buffer[j].begin() + i);

					--i;
					continue;
				}
			}

			if (buffer[3][i]) break;
		}
	if (buffer[0].size() == 0) return 0;
	else return 1;
}