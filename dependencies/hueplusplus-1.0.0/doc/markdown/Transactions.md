# Transactions {#transactions}

[TOC]

## Using a transaction for lights
Often, you want to change more than one property on a light at the same time,
for example brightness and color. This is done using transactions ([StateTransaction](@ref hueplusplus::StateTransaction)).
\snippet Snippets.cpp transaction-lights

The request is reduced to only the variables that need to be changed based on the current state.
For example, if the light is already on, that part of the transaction is ignored.

__Important:__ The transaction has an internal reference to the light state. 
You must not cause a refresh of the state between creating and committing the transaction (e.g. non-const getters/setters), 
because that invalidates the reference.

### Advanced usage
Another way to use the transaction is by storing it and building up the calls separately.
\snippet Snippets.cpp transaction-advanced

In this case, it is especially important that the light and the state of the light __MUST NOT__ invalidate. That means
* the light variable has to live longer than the transaction
* especially no non-const method calls on the light while the transaction is open, or committing other transactions

In general, this method is easier to screw up and should only be used when really necessary.

## Using a transaction for groups
The same principles of transactions for lights also apply for groups. The main difference is that
for groups, there are no checks of the current state. Even if all lights in the group are already on,
the request to turn on all lights on the group is still sent.
\snippet Snippets.cpp transaction-groups


## Creating Actions
In a [Schedule](@ref hueplusplus::Schedule) or [Rule](@ref hueplusplus::Rule),
the bridge can set the state of lights and groups. To configure this, a transaction
can be saved for later instead of committing it directly.
\snippet Snippets.cpp transaction-action
