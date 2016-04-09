#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include <memory>

using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

static cl::OptionCategory AutoRejectCategory("Fails the build if auto is detected");

/* when we see a type declaration, vet it for auto */
class AutoRejectVisitor
 : public RecursiveASTVisitor<AutoRejectVisitor> {
public:
  explicit AutoRejectVisitor(ASTContext *Context)
    : Context(Context) { }
  virtual bool VisitType(Type *t) {
    if (t->isUndeducedType()) {
      outs() << "Found auto! Please don't use auto in your code!\n";
      return false;
    }
    return true;
  }
private:
  ASTContext *Context;
};

/* just recurse through every declaration we see */
class AutoRejectConsumer : public ASTConsumer {
public:
  explicit AutoRejectConsumer(ASTContext *Context)
    : visitor(Context) { }

  virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
    for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
      Decl *D = *i;
      visitor.TraverseDecl(D);
    }
    return true; 
  }
private:
  AutoRejectVisitor visitor;
};

class AutoRejectFrontendAction : public ASTFrontendAction {
public:
  virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
    return std::unique_ptr<ASTConsumer>(new AutoRejectConsumer(&CI.getASTContext()));
  }
};

int main(int argc, const char **argv) {
  CommonOptionsParser op(argc, argv, AutoRejectCategory);
  ClangTool Tool(op.getCompilations(), op.getSourcePathList());
  int result = Tool.run(newFrontendActionFactory<AutoRejectFrontendAction>().get());
  return result;
}
