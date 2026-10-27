#include <iostream>
#include <memory>
#include <string>
#include <cmath>
#include <utility>
#include <iomanip>
using namespace std;
class Expression {
public:
    virtual ~Expression() {}
    virtual double evaluate(double x) const = 0;
    virtual shared_ptr<Expression> derivative() const = 0;
    virtual string toString() const = 0;
};
class Constant : public Expression {
    double value;
public:
    Constant(double v):value(v) {}
    double evaluate(double x) const override {
        return value;
    }
    shared_ptr<Expression> derivative() const override {
        return make_shared<Constant>(0);
    }
    string toString() const override {
        if (value == (int)value) {
            return to_string((int)value);
        }
        return to_string(value);
    }
};
class Variable:public Expression {
public:
    double evaluate(double x) const override {
        return x;
    }
    shared_ptr<Expression> derivative() const override {
        return make_shared<Constant>(1);
    }
    string toString() const override {
        return "x";
    }
};
class BinaryOperation:public Expression {
protected:
    shared_ptr<Expression> left;
    shared_ptr<Expression> right;
public:
    BinaryOperation(shared_ptr<Expression> l,shared_ptr<Expression> r):left(move(l)),right(move(r)) {}
};
class UnaryFunction:public Expression {
protected:
    shared_ptr<Expression> argument;
public:
    explicit UnaryFunction(shared_ptr<Expression> a):argument(move(a)) {}
};
class Add:public BinaryOperation {
public:
    Add(shared_ptr<Expression> l,shared_ptr<Expression> r):BinaryOperation(move(l),move(r)) {}
    double evaluate(double x) const override {
        return left->evaluate(x)+right->evaluate(x);
    }
    shared_ptr<Expression> derivative() const override {
        return make_shared<Add>(left->derivative(),right->derivative());
    }
    string toString() const override {
        return "("+left->toString()+" + "+right->toString()+")";
    }
};
class Multiply:public BinaryOperation {
public:
    Multiply(shared_ptr<Expression> l,shared_ptr<Expression> r):BinaryOperation(move(l),move(r)) {}
    double evaluate(double x) const override {
        return left->evaluate(x)*right->evaluate(x);
    }
    shared_ptr<Expression> derivative() const override {
        // Regla del producto: (u*v)' = u'*v + u*v'
        return make_shared<Add>(make_shared<Multiply>(left->derivative(),right),make_shared<Multiply>(left,right->derivative()));
    }
    string toString() const override {
        return "("+left->toString()+" * "+right->toString()+")";
    }
};
class Power:public BinaryOperation {
public:
    Power(shared_ptr<Expression> l,shared_ptr<Expression> r):BinaryOperation(move(l),move(r)) {}
    double evaluate(double x) const override {
        return pow(left->evaluate(x),right->evaluate(x));
    }
    shared_ptr<Expression> derivative() const override {
        // Para exponentes constantes: (u^n)' = n*u^(n-1)*u'
        // Esta versión está pensada para el ejercicio con x^2.
        double exponent=right->evaluate(0);
        if (exponent==0) {
            return make_shared<Constant>(0);
        }
        return make_shared<Multiply>(make_shared<Multiply>(make_shared<Constant>(exponent),make_shared<Power>(left,make_shared<Constant>(exponent - 1))),left->derivative());
    }
    string toString() const override {
        return "("+left->toString()+" ^ "+right->toString()+")";
    }
};
class Divide:public BinaryOperation {
public:
    Divide(shared_ptr<Expression> l,shared_ptr<Expression> r):BinaryOperation(move(l),move(r)) {}
    double evaluate(double x) const override {
        double denominator=right->evaluate(x);
        if (denominator==0) {
            throw runtime_error("Division entre cero");
        }
        return left->evaluate(x)/denominator;
    }
    shared_ptr<Expression> derivative() const override {
        // Regla del cociente: (u/v)' = (u'v - uv')/v^2
        return make_shared<Divide>(make_shared<Add>(make_shared<Multiply>(left->derivative(),right),make_shared<Multiply>(make_shared<Constant>(-1),make_shared<Multiply>(left,right->derivative()))),make_shared<Power>(right,make_shared<Constant>(2)));
    }
    string toString() const override {
        return "("+left->toString()+" / "+right->toString()+")";
    }
};
class Sin;
class Cos;
class Sin:public UnaryFunction {
public:
    explicit Sin(shared_ptr<Expression> a):UnaryFunction(move(a)) {}
    double evaluate(double x) const override {
        return sin(argument->evaluate(x));
    }
    shared_ptr<Expression> derivative() const override;
    string toString() const override {
        return "sin("+argument->toString()+")";
    }
};
class Cos:public UnaryFunction {
public:
    explicit Cos(shared_ptr<Expression> a):UnaryFunction(move(a)) {}
    double evaluate(double x) const override {
        return cos(argument->evaluate(x));
    }
    shared_ptr<Expression> derivative() const override;
    string toString() const override {
        return "cos("+argument->toString()+")";
    }
};
// Derivada del seno: cos(u)*u'
shared_ptr<Expression> Sin::derivative() const {
    return make_shared<Multiply>(make_shared<Cos>(argument),argument->derivative());
}
// Derivada del coseno: -sin(u)*u'
shared_ptr<Expression> Cos::derivative() const {
    return make_shared<Multiply>(make_shared<Constant>(-1),make_shared<Multiply>(make_shared<Sin>(argument),argument->derivative()));
}
int main() {
    auto x=make_shared<Variable>();
    auto expr=make_shared<Multiply>(make_shared<Add>(make_shared<Power>(x,make_shared<Constant>(2)),make_shared<Multiply>(make_shared<Constant>(3),x)),make_shared<Sin>(x));
    auto deriv=expr->derivative();
    cout<<fixed<<setprecision(2);
    cout<<"f(x) = "<<expr->toString()<<endl;
    cout<<"f'(x) = "<<deriv->toString()<<endl;
    cout<<"f(2) = "<<expr->evaluate(2)<<endl;
    cout<<"f'(2) = "<<deriv->evaluate(2)<<endl;
    return 0;
}