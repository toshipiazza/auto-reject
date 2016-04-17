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
#include "clang/Frontend/FrontendPluginRegistry.h"

#include <memory>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

/* when we see a type declaration, vet it for auto */
class AutoRejectVisitor
 : public RecursiveASTVisitor<AutoRejectVisitor> {
public:
  explicit AutoRejectVisitor(ASTContext *Context)
    : Context(Context) { }
  bool VisitValueDecl(ValueDecl *Declaration) {
    FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getLocStart());
    if (!FullLocation.isValid()) {
      return true;
    } else if (FullLocation.isInSystemHeader()) {
      /* we don't care about system headers containing auto */
      return true;
    } else if (!Declaration->getType().getTypePtr()->getContainedAutoType()) {
      /* if the declaration had no auto type, we don't care */
      return true;
    }

    errs() << "Found declaration of auto at "
           << FullLocation.getManager().getFilename(FullLocation) << " "
           << FullLocation.getSpellingLineNumber() << ":"
           << FullLocation.getSpellingColumnNumber() << "\n";
    return false;
  }
private:
  ASTContext *Context;
};

class AutoRejectConsumer : public ASTConsumer {
public:
  explicit AutoRejectConsumer(ASTContext *Context)
    : visitor(Context) { }
  virtual void HandleTranslationUnit(ASTContext &Context) {
    visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  AutoRejectVisitor visitor;
};

class AutoRejectFrontendAction : public PluginASTAction {
public:
  virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                         StringRef file) {
    return std::unique_ptr<ASTConsumer>(
        new AutoRejectConsumer(&CI.getASTContext()));
  }
  virtual bool ParseArgs(const CompilerInstance &CI,
                         const std::vector<std::string>& args) {
    return true;
  }
};

static FrontendPluginRegistry::Add<AutoRejectFrontendAction> X("-auto-reject",
    "Rejects any code that uses auto");
