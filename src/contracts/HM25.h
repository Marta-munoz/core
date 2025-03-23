using namespace QPI;

struct MilestoneContract : public ContractBase
{
public:
    struct RegisterUser_input {
        qstring user;
    };
    struct RegisterUser_output {};

    struct AddMilestone_input {
        qstring user;
        uint64 reward;
    };
    struct AddMilestone_output {};

    struct ClaimReward_input {
        qstring user;
    };
    struct ClaimReward_output {
        uint64 amountPaid;
    };

    struct GetUserRewards_input {
        qstring user;
    };
    struct GetUserRewards_output {
        uint64 pendingReward;
    };

private:
    std::map<qstring, uint64> userRewards;

    /**
    * Register a user in the system
    */
    PUBLIC_PROCEDURE(RegisterUser)
        userRewards[input.user] = 0;
    _

    /**
    * Add a milestone reward for a user
    */
    PUBLIC_PROCEDURE(AddMilestone)
        userRewards[input.user] += input.reward;
    _

    /**
    * Allow user to claim their reward
    */
    PUBLIC_PROCEDURE(ClaimReward)
        uint64 reward = userRewards[input.user];
        if (reward > 0)
        {
            userRewards[input.user] = 0;
            qpi.transfer(input.user, reward);
            output.amountPaid = reward;
        }
        else
        {
            output.amountPaid = 0;
        }
    _

    /**
    * Get user's pending rewards
    */
    PUBLIC_FUNCTION(GetUserRewards)
        output.pendingReward = userRewards[input.user];
    _

    REGISTER_USER_FUNCTIONS_AND_PROCEDURES
        REGISTER_USER_PROCEDURE(RegisterUser, 1);
        REGISTER_USER_PROCEDURE(AddMilestone, 2);
        REGISTER_USER_PROCEDURE(ClaimReward, 3);
        REGISTER_USER_FUNCTION(GetUserRewards, 1);
    _

    INITIALIZE
        userRewards.clear();
    _
};
