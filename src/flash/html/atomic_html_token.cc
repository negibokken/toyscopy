class HTMLToken {
  public:
  enum Type {
    Uninitialized,
    DOCTYPE,
    StartTag,
    EndTag,
    Comment,
    Character,
    EndOfFile
  };

  Type type;
  HTMLToken(Type _type) : type(_type) {}
};
