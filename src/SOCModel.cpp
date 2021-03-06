#include <memory>
#include <stdexcept>
#include <string>
using namespace std::string_literals;

#include <yaml-cpp/yaml.h>
#include <armadillo>

#include "SOCModel.h"
#include "Misc.h"

namespace YAML {

template <>
std::unique_ptr<SOCModel::Base> Node::as() const {
	auto type_name = Misc::mapat(*this,"type").as<std::string>();
	try{
	return SOCModel::Base::get_factories()
	       .at(type_name)
	       ->create_from_YAML(*this);
	} catch (const std::out_of_range&) {
		throw YAML::RepresentationException(
		    this->Mark(), "Unrecognized type: "s + type_name);
	}
}

}  // namespace YAML

namespace SOCModel {

arma::vec3 Isotropic3D::omega(const arma::vec3& k) const { return o*k; }

arma::vec3 Dresselhaus::omega(const arma::vec3& k) const {
	return o*arma::vec{
		k[0] * (k[1] * k[1] - k[2] * k[2]),
		k[1] * (k[2] * k[2] - k[0] * k[0]),
		k[2] * (k[0] * k[0] - k[1] * k[1]),
	};
}

arma::vec3 Zeeman::omega(const arma::vec3& k) const {
	return base_model->omega(k) + bfield;
}

arma::vec3 Stretch::omega(const arma::vec3& k) const {
	return base_model->omega(k) % lambdas;  // Elementwise multiplication
}

}  // namespace SOCModel

template class RegisterSubclass2<SOCModel::Isotropic3D,
				 SOCModel::Subclass_policy>;
template class RegisterSubclass2<SOCModel::Dresselhaus,
				 SOCModel::Subclass_policy>;
template class RegisterSubclass2<SOCModel::Zeeman,
				 SOCModel::Subclass_policy>;
template class RegisterSubclass2<SOCModel::Stretch,
				 SOCModel::Subclass_policy>;
