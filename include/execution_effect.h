//
// Created by Артём Линт on 13.08.2025.
//

#ifndef CPPSHELL_EXECUTIONEFFECT_H
#define CPPSHELL_EXECUTIONEFFECT_H

#include <arguments.h>
#include <functional>
#include <utility>
#include <vector>
#include <string>
#include <unordered_map>


class ExecutionEffect {
public:
	virtual ~ExecutionEffect() = default;

	virtual void commenceEffect() = 0;
	virtual void cleanup() = 0;

};




namespace Effect {
	class Redirect final : public ExecutionEffect {
		ArgumentList arguments;
	public:

		explicit Redirect(const ArgumentList& args) {
			arguments = args;
		};

			void commenceEffect() override;
			void cleanup() override;

		~Redirect() override = default;
		};
	};

// 8====D<----

using EffectFactory = std::function<ExecutionEffect*(ArgumentList args)>;

inline std::unordered_map<std::string, EffectFactory> effectsRegistry = {
	{">", [](const ArgumentList &args) -> ExecutionEffect*{
		return new Effect::Redirect(args);
	}, }
};
#endif //CPPSHELL_EXECUTIONEFFECT_H
