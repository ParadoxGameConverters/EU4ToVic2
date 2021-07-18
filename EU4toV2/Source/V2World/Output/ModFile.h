#ifndef MOD_FILE_H
#define MOD_FILE_H

namespace V2
{
class ModFile
{
  public:
	friend std::ostream& operator<<(std::ostream& output, const ModFile& modFile);
};
} // namespace V2
#endif // MOD_FILE_H
