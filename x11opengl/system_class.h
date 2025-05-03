#ifndef _SYSTEM_CLASS_H_
#define _SYSTEM_CLASS_H_
class SystemClass
{
public:
  SystemClass();
  SystemClass(const SystemClass&);
  ~SystemClass();

  bool Initialize();
  void Shutdown();
  void Frame();
private:
};
#endif
