#ifndef MAGNETIC_FIELD_H
#define MAGNETIC_FIELD_H

#include <map>
#include <memory>
#include <string>

#include <yaml-cpp/yaml.h>
#include <armadillo>

#include "RegisterSubclass.h"

namespace MagneticField {

class Base {
       public:
	class Factory {
	       public:
		virtual std::unique_ptr<Base> create_from_YAML(
		    const YAML::Node&) = 0;
		virtual ~Factory() {}
	};

       private:
	static auto& factories() {
		static std::map<std::string, std::unique_ptr<Factory>> f;
		return f;
	};
	template <typename Base, typename Child>
	friend RegisterSubclass<Base, Child>::Register::Register();

       public:
	virtual const arma::vec3 advance(const arma::vec3& s0, double t0,
					 double t,
					 const arma::vec3& bconst) = 0;
	virtual ~Base() {}
};

class Zero : public Base {
       public:
	static constexpr char type_name[] = "Zero";
	class Factory : public Base::Factory {
	       public:
		virtual std::unique_ptr<Base> create_from_YAML(
		    const YAML::Node&) override;
	};
	const arma::vec3 advance(const arma::vec3& s0, double t0, double t,
				 const arma::vec3& bconst) override;
};

class Step : public Base {
       private:
	arma::vec3 field;
	double tstep;

       public:
	static constexpr char type_name[] = "Zero";
	class Factory : public Base::Factory {
	       public:
		virtual std::unique_ptr<Base> create_from_YAML(
		    const YAML::Node&) override;
	};
	Step() = delete;
	inline Step(const arma::vec3& field, double tstep)
	    : field(field), tstep(tstep) {}
	const arma::vec3 advance(const arma::vec3& s0, double t0, double t,
				 const arma::vec3& bconst) override;
};
}  // namespace MagneticField
template class RegisterSubclass<MagneticField::Base, MagneticField::Zero>;
template class RegisterSubclass<MagneticField::Base, MagneticField::Step>;

#endif  // MAGNETIC_FIELD_H
